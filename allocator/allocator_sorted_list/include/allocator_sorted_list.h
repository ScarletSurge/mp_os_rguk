#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>

class allocator_sorted_list final :
    private allocator_guardant,
    public allocator_test_utils,
    public allocator_with_fit_mode,
    private logger_guardant,
    private typename_holder
{

private:

    void* _trusted_memory;

public:

    ~allocator_sorted_list() noexcept override;

    allocator_sorted_list(
        allocator_sorted_list const& other) = delete;

    allocator_sorted_list& operator=(
        allocator_sorted_list const& other) = delete;

    allocator_sorted_list(
        allocator_sorted_list&& other) noexcept;

    allocator_sorted_list& operator=(
        allocator_sorted_list&& other) noexcept;

public:

    explicit allocator_sorted_list(
        size_t space_size,
        allocator* parent_allocator = nullptr,
        logger* logger = nullptr,
        allocator_with_fit_mode::fit_mode allocate_fit_mode = allocator_with_fit_mode::fit_mode::first_fit);

public:

    [[nodiscard]] void* allocate(
        size_t value_size,
        size_t values_count) override;

    void deallocate(
        void* at) override;

public:

    inline void set_fit_mode(
        allocator_with_fit_mode::fit_mode mode) override;

private:

    inline allocator* get_allocator() const override;

public:

    std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept override;

private:

    inline logger* get_logger() const override;

private:

    inline std::string get_typename() const noexcept override;

private:

    size_t &obtain_trusted_memory_size() const;

    static size_t constexpr common_metadata_size();

    static size_t constexpr available_block_metadata_size();

    static size_t constexpr ancillary_block_metadata_size();

    std::mutex& obtain_synchronizer() const;

    void*& obtain_first_available_block_address_byref() const;

    void** obtain_first_available_block_address_byptr() const;

    static void *&obtain_allocator_trusted_memory_ancillary_block_owner(
        void *current_ancillary_block_address);

    static size_t &obtain_ancillary_block_size(
        void *current_ancillary_block_address);

    static void*& obtain_next_available_block_address(
        void* current_available_block_address);

    static size_t& obtain_available_block_size(
        void* current_available_block_address);

    allocator_with_fit_mode::fit_mode& obtain_fit_mode() const;

    inline void throw_if_allocator_instance_state_was_moved() const;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H