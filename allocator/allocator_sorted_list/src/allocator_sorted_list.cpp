#include <mutex>

#include <not_implemented.h>

#include "../include/allocator_sorted_list.h"

allocator_sorted_list::~allocator_sorted_list() noexcept
{
    if (_trusted_memory == nullptr)
    {
        return;
    }

    // obtain_synchronizer().~mutex();
    allocator::destruct(&obtain_synchronizer());

    this
        ->information_with_guard("...")
        ->warning_with_guard("...")
        ->debug_with_guard("...");

    deallocate_with_guard(_trusted_memory);
}

allocator_sorted_list::allocator_sorted_list(
    allocator_sorted_list &&other) noexcept
{
    throw not_implemented("allocator_sorted_list::allocator_sorted_list(allocator_sorted_list &&) noexcept", "your code should be here...");
}

allocator_sorted_list &allocator_sorted_list::operator=(
    allocator_sorted_list &&other) noexcept
{
    throw not_implemented("allocator_sorted_list &allocator_sorted_list::operator=(allocator_sorted_list &&) noexcept", "your code should be here...");
}

allocator_sorted_list::allocator_sorted_list(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (space_size < available_block_metadata_size())
    {
        throw std::logic_error("Can't initialize allocator instance");
    }

    size_t memory_size = space_size + summ_size();
    try
    {
        _trusted_memory = parent_allocator == nullptr
            ? ::operator new (memory_size)
            : parent_allocator->allocate(1, memory_size);
    }
    catch (std::bad_alloc const &ex)
    {
        // TODO: логи очень хотят, чтобы их записали =)

        throw;
    }

    allocator **parent_allocator_placement = reinterpret_cast<allocator **>(_trusted_memory);
    *parent_allocator_placement = parent_allocator;

    class logger **logger_placement = reinterpret_cast<class logger **>(parent_allocator_placement + 1);
    *logger_placement = logger;

    std::mutex *synchronizer_placement = reinterpret_cast<std::mutex *>(logger_placement + 1);
    new (reinterpret_cast<void *>(synchronizer_placement)) std::mutex();
    // allocator::construct(synchronizer_placement);

    unsigned char *placement = reinterpret_cast<unsigned char *>(synchronizer_placement);

    placement += sizeof(std::mutex);
    *reinterpret_cast<allocator_with_fit_mode::fit_mode *>(placement) = allocate_fit_mode;

    placement += sizeof(allocator_with_fit_mode::fit_mode);
    *reinterpret_cast<size_t *>(placement) = space_size;

    placement += sizeof(size_t);
    *reinterpret_cast<void **>(placement) = placement + sizeof(void *);

    // BEFORE:
    //*reinterpret_cast<void **>(*reinterpret_cast<void **>(placement)) = nullptr;

    // AFTER:
    obtain_next_available_block_address(obtain_first_available_block_address_byref()) = nullptr;

    *reinterpret_cast<size_t *>(reinterpret_cast<void **>(*reinterpret_cast<void **>(placement)) + 1) = space_size - available_block_metadata_size();

    // TODO: логи всё ещё очень хотят, чтобы их записали =)
}

[[nodiscard]] void *allocator_sorted_list::allocate(
    size_t value_size,
    size_t values_count)
{
    std::lock_guard<std::mutex> lock(obtain_synchronizer());

    void *target_block = nullptr, *previous_to_target_block = nullptr;
    size_t requested_size = value_size * values_count + ancillary_block_metadata_size();
    size_t target_block_size;

    {
        void *current_block, *previous_block = nullptr;
        current_block = obtain_first_available_block_address_byref();
        allocator_with_fit_mode::fit_mode fit_mode = obtain_fit_mode();

        while (current_block != nullptr)
        {
            size_t current_block_size = obtain_available_block_size(current_block);

            if (current_block_size >= requested_size && (
                fit_mode == allocator_with_fit_mode::fit_mode::first_fit ||
                (fit_mode == allocator_with_fit_mode::fit_mode::the_best_fit &&
                 (target_block == nullptr || current_block_size < target_block_size)) ||
                (fit_mode == allocator_with_fit_mode::fit_mode::the_worst_fit &&
                 (target_block == nullptr || current_block_size > target_block_size))))
            {
                target_block = current_block;
                previous_to_target_block = previous_block;
                target_block_size = current_block_size;

                if (fit_mode == allocator_with_fit_mode::fit_mode::first_fit)
                {
                    break;
                }
            }

            previous_block = current_block;
            current_block = obtain_next_available_block_address(current_block);
        }
    }

    if (target_block == nullptr)
    {
        // TODO: логи типо такие: напишите нас плиз =)

        throw std::bad_alloc();
    }

    // TODO: You can do it! :)

    return reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(target_block) + ancillary_block_metadata_size());
}

void allocator_sorted_list::deallocate(
    void *at)
{
    std::lock_guard<std::mutex> lock(obtain_synchronizer());

    // TODO: You also can do it :)
}

inline void allocator_sorted_list::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    throw not_implemented("inline void allocator_sorted_list::set_fit_mode(allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

inline allocator *allocator_sorted_list::get_allocator() const
{
    return *reinterpret_cast<allocator **>(_trusted_memory);
}

std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept
{
    throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept", "your code should be here...");
}

inline logger *allocator_sorted_list::get_logger() const
{
    return *(reinterpret_cast<logger **>(&obtain_synchronizer()) - 1);
}

inline std::string allocator_sorted_list::get_typename() const noexcept
{
    throw not_implemented("inline std::string allocator_sorted_list::get_typename() const noexcept", "your code should be here...");
}

size_t constexpr allocator_sorted_list::summ_size()
{
    return sizeof(allocator *) + sizeof(std::mutex) + sizeof(logger *) + sizeof( allocator_with_fit_mode::fit_mode) + sizeof(size_t) + sizeof(void *);
}

size_t constexpr allocator_sorted_list::available_block_metadata_size()
{
    return sizeof(size_t) + sizeof(void *);
}

size_t constexpr allocator_sorted_list::ancillary_block_metadata_size()
{
    // return sizeof(size_t) + sizeof(void *);
    return sizeof(block_size_t) + sizeof(void *);
}

std::mutex &allocator_sorted_list::obtain_synchronizer() const
{
    return *reinterpret_cast<std::mutex *>(const_cast<unsigned char *>(reinterpret_cast<unsigned char const *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *)));
}

void *&allocator_sorted_list::obtain_first_available_block_address_byref() const
{
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(std::mutex) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(size_t));
}

void **allocator_sorted_list::obtain_first_available_block_address_byptr() const
{
    return reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(std::mutex) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(size_t));
}

void *&allocator_sorted_list::obtain_next_available_block_address(
    void *current_available_block_address)
{
    return *reinterpret_cast<void **>(current_available_block_address);
}

size_t &allocator_sorted_list::obtain_available_block_size(
    void *current_available_block_address)
{
    return *reinterpret_cast<size_t *>(&obtain_next_available_block_address(current_available_block_address) + 1);

    // return *reinterpret_cast<size_t *>(reinterpret_cast<void **>(current_available_block_address) + 1);

    // return *reinterpret_cast<size_t *>(reinterpret_cast<unsigned char *>(current_available_block_address) + sizeof(void *));
}

allocator_with_fit_mode::fit_mode &allocator_sorted_list::obtain_fit_mode() const
{
    return *reinterpret_cast<allocator_with_fit_mode::fit_mode *>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(allocator *) + sizeof(logger *) + sizeof(std::mutex));
}