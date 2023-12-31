function(target_sources_if_not_overridden filename)
    list(FIND TX_SRC_OVERRIDES ${filename} OVERRIDE_FOUND)
    if( OVERRIDE_FOUND EQUAL -1 )
        # message(STATUS "** Using original ${filename} from common/src **")
        target_sources(${PROJECT_NAME} PRIVATE src/${filename})
    endif()
endfunction()

# These files can be overridden by setting them in the variable list named TX_SRC_OVERRIDES
target_sources_if_not_overridden("tx_thread_delete.c")
target_sources_if_not_overridden("tx_thread_reset.c")

target_sources(${PROJECT_NAME}
    PRIVATE
    # {{BEGIN_TARGET_SOURCES}}
		src/tx_block_allocate.c
		src/tx_block_pool_cleanup.c
		src/tx_block_pool_create.c
		src/tx_block_pool_delete.c
		src/tx_block_pool_info_get.c
		src/tx_block_pool_initialize.c
		src/tx_block_pool_performance_info_get.c
		src/tx_block_pool_performance_system_info_get.c
		src/tx_block_pool_prioritize.c
		src/tx_block_release.c
		src/tx_byte_allocate.c
		src/tx_byte_pool_cleanup.c
		src/tx_byte_pool_create.c
		src/tx_byte_pool_delete.c
		src/tx_byte_pool_info_get.c
		src/tx_byte_pool_initialize.c
		src/tx_byte_pool_performance_info_get.c
		src/tx_byte_pool_performance_system_info_get.c
		src/tx_byte_pool_prioritize.c
		src/tx_byte_pool_search.c
		src/tx_byte_release.c
		src/tx_event_flags_cleanup.c
		src/tx_event_flags_create.c
		src/tx_event_flags_delete.c
		src/tx_event_flags_get.c
		src/tx_event_flags_info_get.c
		src/tx_event_flags_initialize.c
		src/tx_event_flags_performance_info_get.c
		src/tx_event_flags_performance_system_info_get.c
		src/tx_event_flags_set.c
		src/tx_event_flags_set_notify.c
		src/tx_initialize_high_level.c
		src/tx_initialize_kernel_enter.c
		src/tx_initialize_kernel_setup.c
		src/tx_misra.c
		src/tx_mutex_cleanup.c
		src/tx_mutex_create.c
		src/tx_mutex_delete.c
		src/tx_mutex_get.c
		src/tx_mutex_info_get.c
		src/tx_mutex_initialize.c
		src/tx_mutex_performance_info_get.c
		src/tx_mutex_performance_system_info_get.c
		src/tx_mutex_prioritize.c
		src/tx_mutex_priority_change.c
		src/tx_mutex_put.c
		src/tx_queue_cleanup.c
		src/tx_queue_create.c
		src/tx_queue_delete.c
		src/tx_queue_flush.c
		src/tx_queue_front_send.c
		src/tx_queue_info_get.c
		src/tx_queue_initialize.c
		src/tx_queue_performance_info_get.c
		src/tx_queue_performance_system_info_get.c
		src/tx_queue_prioritize.c
		src/tx_queue_receive.c
		src/tx_queue_send.c
		src/tx_queue_send_notify.c
		src/tx_semaphore_ceiling_put.c
		src/tx_semaphore_cleanup.c
		src/tx_semaphore_create.c
		src/tx_semaphore_delete.c
		src/tx_semaphore_get.c
		src/tx_semaphore_info_get.c
		src/tx_semaphore_initialize.c
		src/tx_semaphore_performance_info_get.c
		src/tx_semaphore_performance_system_info_get.c
		src/tx_semaphore_prioritize.c
		src/tx_semaphore_put.c
		src/tx_semaphore_put_notify.c
		src/tx_thread_create.c
		src/tx_thread_entry_exit_notify.c
		src/tx_thread_identify.c
		src/tx_thread_info_get.c
		src/tx_thread_initialize.c
		src/tx_thread_performance_info_get.c
		src/tx_thread_performance_system_info_get.c
		src/tx_thread_preemption_change.c
		src/tx_thread_priority_change.c
		src/tx_thread_relinquish.c
		src/tx_thread_resume.c
		src/tx_thread_shell_entry.c
		src/tx_thread_sleep.c
		src/tx_thread_stack_analyze.c
		src/tx_thread_stack_error_handler.c
		src/tx_thread_stack_error_notify.c
		src/tx_thread_suspend.c
		src/tx_thread_system_preempt_check.c
		src/tx_thread_system_resume.c
		src/tx_thread_system_suspend.c
		src/tx_thread_terminate.c
		src/tx_thread_time_slice.c
		src/tx_thread_time_slice_change.c
		src/tx_thread_timeout.c
		src/tx_thread_wait_abort.c
		src/tx_time_get.c
		src/tx_time_set.c
		src/tx_timer_activate.c
		src/tx_timer_change.c
		src/tx_timer_create.c
		src/tx_timer_deactivate.c
		src/tx_timer_delete.c
		src/tx_timer_expiration_process.c
		src/tx_timer_info_get.c
		src/tx_timer_initialize.c
		src/tx_timer_performance_info_get.c
		src/tx_timer_performance_system_info_get.c
		src/tx_timer_system_activate.c
		src/tx_timer_system_deactivate.c
		src/tx_timer_thread_entry.c
		src/tx_trace_buffer_full_notify.c
		src/tx_trace_disable.c
		src/tx_trace_enable.c
		src/tx_trace_event_filter.c
		src/tx_trace_event_unfilter.c
		src/tx_trace_initialize.c
		src/tx_trace_interrupt_control.c
		src/tx_trace_isr_enter_insert.c
		src/tx_trace_isr_exit_insert.c
		src/tx_trace_object_register.c
		src/tx_trace_object_unregister.c
		src/tx_trace_user_event_insert.c
		src/txe_block_allocate.c
		src/txe_block_pool_create.c
		src/txe_block_pool_delete.c
		src/txe_block_pool_info_get.c
		src/txe_block_pool_prioritize.c
		src/txe_block_release.c
		src/txe_byte_allocate.c
		src/txe_byte_pool_create.c
		src/txe_byte_pool_delete.c
		src/txe_byte_pool_info_get.c
		src/txe_byte_pool_prioritize.c
		src/txe_byte_release.c
		src/txe_event_flags_create.c
		src/txe_event_flags_delete.c
		src/txe_event_flags_get.c
		src/txe_event_flags_info_get.c
		src/txe_event_flags_set.c
		src/txe_event_flags_set_notify.c
		src/txe_mutex_create.c
		src/txe_mutex_delete.c
		src/txe_mutex_get.c
		src/txe_mutex_info_get.c
		src/txe_mutex_prioritize.c
		src/txe_mutex_put.c
		src/txe_queue_create.c
		src/txe_queue_delete.c
		src/txe_queue_flush.c
		src/txe_queue_front_send.c
		src/txe_queue_info_get.c
		src/txe_queue_prioritize.c
		src/txe_queue_receive.c
		src/txe_queue_send.c
		src/txe_queue_send_notify.c
		src/txe_semaphore_ceiling_put.c
		src/txe_semaphore_create.c
		src/txe_semaphore_delete.c
		src/txe_semaphore_get.c
		src/txe_semaphore_info_get.c
		src/txe_semaphore_prioritize.c
		src/txe_semaphore_put.c
		src/txe_semaphore_put_notify.c
		src/txe_thread_create.c
		src/txe_thread_delete.c
		src/txe_thread_entry_exit_notify.c
		src/txe_thread_info_get.c
		src/txe_thread_preemption_change.c
		src/txe_thread_priority_change.c
		src/txe_thread_relinquish.c
		src/txe_thread_reset.c
		src/txe_thread_resume.c
		src/txe_thread_suspend.c
		src/txe_thread_terminate.c
		src/txe_thread_time_slice_change.c
		src/txe_thread_wait_abort.c
		src/txe_timer_activate.c
		src/txe_timer_change.c
		src/txe_timer_create.c
		src/txe_timer_deactivate.c
		src/txe_timer_delete.c
		src/txe_timer_info_get.c

    # {{END_TARGET_SOURCES}}
)

# Add the Common/inc directory to the project include list
target_include_directories(${PROJECT_NAME} 
    SYSTEM
    PUBLIC
		inc
)

