QT += qml quick

CONFIG += c++1z

SOURCES += main.cpp \
    vrm_core/vrm/core/assert/impl/assert.cpp \
    vrm_core/vrm/core/ostream_utils/nullptr_printer.cpp \
    temp.cpp

RESOURCES +=

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Includepaths for dependencies
INCLUDEPATH += ecst
INCLUDEPATH += vrm_core
INCLUDEPATH += vrm_pp

BOOST_ROOT = "C:/msys64/mingw64/include"
# We also need boost
_BOOST_ROOT = $$BOOST_ROOT
isEmpty(_BOOST_ROOT) {
    message(\"Boost Library\" qmake value not detected...)

    # Try to use the system environment value.
    _BOOST_ROOT = $$(BOOST_ROOT)
}

isEmpty(_BOOST_ROOT) {
    message(\"Boost Library\" environment variable not detected...)
    !build_pass:error(Please set the environment variable `BOOST_ROOT`. For example, BOOST_ROOT=c:\\boost_1_53_0)
} else {
    message(\"Boost Library\" detected in BOOST_ROOT = \"$$_BOOST_ROOT\")
    INCLUDEPATH += $$_BOOST_ROOT
}


DISTFILES += \
    qtquickcontrols2.conf \
    main.qml \
    Hand.qml \
    Avatar.qml \
    Board.qml

HEADERS += \
    ecst/ecst/aliases/threading/third_party/mingw-std-threads/mingw.condition_variable.h \
    ecst/ecst/aliases/threading/third_party/mingw-std-threads/mingw.mutex.h \
    ecst/ecst/aliases/threading/third_party/mingw-std-threads/mingw.thread.h \
    ecst/ecst/aliases/threading/boost.hpp \
    ecst/ecst/aliases/threading/std.hpp \
    ecst/ecst/aliases/assert.hpp \
    ecst/ecst/aliases/attributes.hpp \
    ecst/ecst/aliases/bitset.hpp \
    ecst/ecst/aliases/hana.hpp \
    ecst/ecst/aliases/threading.hpp \
    ecst/ecst/aliases/vrmc.hpp \
    ecst/ecst/config/names/context/bitset/bitset.hpp \
    ecst/ecst/config/names/context/entity/entity.hpp \
    ecst/ecst/config/names/context/storage/component.hpp \
    ecst/ecst/config/names/context/storage/entity.hpp \
    ecst/ecst/config/names/context/storage/storage.hpp \
    ecst/ecst/config/names/context/storage/system.hpp \
    ecst/ecst/config/names/context/system/system.hpp \
    ecst/ecst/config/names/context/bitset.hpp \
    ecst/ecst/config/names/context/context.hpp \
    ecst/ecst/config/names/context/entity.hpp \
    ecst/ecst/config/names/context/storage.hpp \
    ecst/ecst/config/names/context/system.hpp \
    ecst/ecst/config/names/debug/debug.hpp \
    ecst/ecst/config/names/global/global.hpp \
    ecst/ecst/config/names/hardware/hardware.hpp \
    ecst/ecst/config/names/inner_parallelism/composer.hpp \
    ecst/ecst/config/names/inner_parallelism/inner_parallelism.hpp \
    ecst/ecst/config/names/inner_parallelism/strategy.hpp \
    ecst/ecst/config/names/mp/list.hpp \
    ecst/ecst/config/names/mp/mp.hpp \
    ecst/ecst/config/names/mp/option_map.hpp \
    ecst/ecst/config/names/scheduler/atomic_counter.hpp \
    ecst/ecst/config/names/scheduler/scheduler.hpp \
    ecst/ecst/config/names/settings/settings.hpp \
    ecst/ecst/config/names/signature/component.hpp \
    ecst/ecst/config/names/signature/signature.hpp \
    ecst/ecst/config/names/signature/system.hpp \
    ecst/ecst/config/names/signature_list/component.hpp \
    ecst/ecst/config/names/signature_list/signature_list.hpp \
    ecst/ecst/config/names/signature_list/system.hpp \
    ecst/ecst/config/names/system_execution_adapter/system_execution_adapter.hpp \
    ecst/ecst/config/names/system_ids_context/system_ids_context.hpp \
    ecst/ecst/config/names/tag/component.hpp \
    ecst/ecst/config/names/tag/system.hpp \
    ecst/ecst/config/names/tag/tag.hpp \
    ecst/ecst/config/names/context.hpp \
    ecst/ecst/config/names/debug.hpp \
    ecst/ecst/config/names/global.hpp \
    ecst/ecst/config/names/hardware.hpp \
    ecst/ecst/config/names/inner_parallelism.hpp \
    ecst/ecst/config/names/mp.hpp \
    ecst/ecst/config/names/scheduler.hpp \
    ecst/ecst/config/names/settings.hpp \
    ecst/ecst/config/names/signature.hpp \
    ecst/ecst/config/names/signature_list.hpp \
    ecst/ecst/config/names/system_execution_adapter.hpp \
    ecst/ecst/config/names/system_ids_context.hpp \
    ecst/ecst/config/names/tag.hpp \
    ecst/ecst/config/names.hpp \
    ecst/ecst/context/bitset/bits.hpp \
    ecst/ecst/context/bitset/builder.hpp \
    ecst/ecst/context/bitset/data.hpp \
    ecst/ecst/context/data/data/refresh_event/refresh_event.hpp \
    ecst/ecst/context/data/data/data.hpp \
    ecst/ecst/context/data/data/refresh_event.hpp \
    ecst/ecst/context/data/defer/proxy/proxy.hpp \
    ecst/ecst/context/data/defer/refresh_state/refresh_state.hpp \
    ecst/ecst/context/data/defer/proxy.hpp \
    ecst/ecst/context/data/defer/refresh_state.hpp \
    ecst/ecst/context/data/main_storage/main_storage.hpp \
    ecst/ecst/context/data/step/step.hpp \
    ecst/ecst/context/data/system_manager/system_manager.hpp \
    ecst/ecst/context/data/data.hpp \
    ecst/ecst/context/data/defer.hpp \
    ecst/ecst/context/data/main_storage.hpp \
    ecst/ecst/context/data/make.hpp \
    ecst/ecst/context/data/step.hpp \
    ecst/ecst/context/data/system_manager.hpp \
    ecst/ecst/context/entity/constants/invalid_id.hpp \
    ecst/ecst/context/entity/handle/handle.hpp \
    ecst/ecst/context/entity/metadata/dispatch.hpp \
    ecst/ecst/context/entity/metadata/metadata.hpp \
    ecst/ecst/context/entity/constants.hpp \
    ecst/ecst/context/entity/handle.hpp \
    ecst/ecst/context/entity/metadata.hpp \
    ecst/ecst/context/scheduler/atomic_counter/atomic_counter.hpp \
    ecst/ecst/context/scheduler/atomic_counter/task.hpp \
    ecst/ecst/context/scheduler/atomic_counter/task_dependency_data.hpp \
    ecst/ecst/context/scheduler/atomic_counter/task_group.hpp \
    ecst/ecst/context/scheduler/atomic_counter/utils.hpp \
    ecst/ecst/context/scheduler/atomic_counter.hpp \
    ecst/ecst/context/storage/component/chunk/impl/buffer_base.hpp \
    ecst/ecst/context/storage/component/chunk/impl/dynamic_buffer.hpp \
    ecst/ecst/context/storage/component/chunk/impl/empty.hpp \
    ecst/ecst/context/storage/component/chunk/impl/fixed_buffer.hpp \
    ecst/ecst/context/storage/component/chunk/impl/hash_map.hpp \
    ecst/ecst/context/storage/component/chunk/tuple/chunk.hpp \
    ecst/ecst/context/storage/component/chunk/tuple/chunk_metadata.hpp \
    ecst/ecst/context/storage/component/chunk/type/component_tuple.hpp \
    ecst/ecst/context/storage/component/chunk/type/tuple.hpp \
    ecst/ecst/context/storage/component/chunk/impl.hpp \
    ecst/ecst/context/storage/component/chunk/tuple.hpp \
    ecst/ecst/context/storage/component/chunk/type.hpp \
    ecst/ecst/context/storage/component/chunk/utils.hpp \
    ecst/ecst/context/storage/component/chunk.hpp \
    ecst/ecst/context/storage/component/data.hpp \
    ecst/ecst/context/storage/component/dispatch.hpp \
    ecst/ecst/context/storage/entity/container/base.hpp \
    ecst/ecst/context/storage/entity/container/dynamic.hpp \
    ecst/ecst/context/storage/entity/container/fixed.hpp \
    ecst/ecst/context/storage/entity/container.hpp \
    ecst/ecst/context/storage/entity/data.hpp \
    ecst/ecst/context/storage/entity/dispatch.hpp \
    ecst/ecst/context/storage/system/utils/storage_tuple.hpp \
    ecst/ecst/context/storage/system/data.hpp \
    ecst/ecst/context/storage/system/utils.hpp \
    ecst/ecst/context/storage/component.hpp \
    ecst/ecst/context/storage/entity.hpp \
    ecst/ecst/context/storage/system.hpp \
    ecst/ecst/context/system/instance/data_proxy/impl/base.hpp \
    ecst/ecst/context/system/instance/data_proxy/impl/multi.hpp \
    ecst/ecst/context/system/instance/data_proxy/impl/single.hpp \
    ecst/ecst/context/system/instance/data_proxy/make/multi.hpp \
    ecst/ecst/context/system/instance/data_proxy/make/single.hpp \
    ecst/ecst/context/system/instance/data_proxy/impl.hpp \
    ecst/ecst/context/system/instance/data_proxy/make.hpp \
    ecst/ecst/context/system/instance/executor_proxy/impl.hpp \
    ecst/ecst/context/system/instance/executor_proxy/make.hpp \
    ecst/ecst/context/system/instance/base.hpp \
    ecst/ecst/context/system/instance/data_proxy.hpp \
    ecst/ecst/context/system/instance/executor_proxy.hpp \
    ecst/ecst/context/system/instance/instance.hpp \
    ecst/ecst/context/system/state/state_manager/data.hpp \
    ecst/ecst/context/system/state/state_manager/state_storage.hpp \
    ecst/ecst/context/system/state/state.hpp \
    ecst/ecst/context/system/state/state_manager.hpp \
    ecst/ecst/context/system/instance.hpp \
    ecst/ecst/context/system/state.hpp \
    ecst/ecst/context/types/types.hpp \
    ecst/ecst/context/bitset.hpp \
    ecst/ecst/context/context.hpp \
    ecst/ecst/context/data.hpp \
    ecst/ecst/context/entity.hpp \
    ecst/ecst/context/scheduler.hpp \
    ecst/ecst/context/storage.hpp \
    ecst/ecst/context/system.hpp \
    ecst/ecst/context/types.hpp \
    ecst/ecst/debug/log/elog.hpp \
    ecst/ecst/debug/log/impl.hpp \
    ecst/ecst/debug/log/interface.hpp \
    ecst/ecst/debug/log.hpp \
    ecst/ecst/hardware/status/core_count.hpp \
    ecst/ecst/hardware/status.hpp \
    ecst/ecst/inner_parallelism/composer/fixed_threshold/executor.hpp \
    ecst/ecst/inner_parallelism/composer/fixed_threshold/make.hpp \
    ecst/ecst/inner_parallelism/composer/fixed_threshold/parameters.hpp \
    ecst/ecst/inner_parallelism/composer/none_below_threshold/make.hpp \
    ecst/ecst/inner_parallelism/composer/fixed_threshold.hpp \
    ecst/ecst/inner_parallelism/composer/none_below_threshold.hpp \
    ecst/ecst/inner_parallelism/strategy/none/executor.hpp \
    ecst/ecst/inner_parallelism/strategy/none/make.hpp \
    ecst/ecst/inner_parallelism/strategy/none/parameters.hpp \
    ecst/ecst/inner_parallelism/strategy/split_evenly/make.hpp \
    ecst/ecst/inner_parallelism/strategy/split_evenly_fn/executor.hpp \
    ecst/ecst/inner_parallelism/strategy/split_evenly_fn/make.hpp \
    ecst/ecst/inner_parallelism/strategy/split_evenly_fn/parameters.hpp \
    ecst/ecst/inner_parallelism/strategy/split_every_n/executor.hpp \
    ecst/ecst/inner_parallelism/strategy/split_every_n/make.hpp \
    ecst/ecst/inner_parallelism/strategy/split_every_n/parameters.hpp \
    ecst/ecst/inner_parallelism/strategy/none.hpp \
    ecst/ecst/inner_parallelism/strategy/split_evenly.hpp \
    ecst/ecst/inner_parallelism/strategy/split_evenly_fn.hpp \
    ecst/ecst/inner_parallelism/strategy/split_every_n.hpp \
    ecst/ecst/inner_parallelism/types/executor_type.hpp \
    ecst/ecst/inner_parallelism/utils/execute_split.hpp \
    ecst/ecst/inner_parallelism/composer.hpp \
    ecst/ecst/inner_parallelism/strategy.hpp \
    ecst/ecst/inner_parallelism/types.hpp \
    ecst/ecst/inner_parallelism/utils.hpp \
    ecst/ecst/mp/core/aliases.hpp \
    ecst/ecst/mp/core/comparers.hpp \
    ecst/ecst/mp/core/is_specialization_of.hpp \
    ecst/ecst/mp/core/type_wrapper.hpp \
    ecst/ecst/mp/list/basic.hpp \
    ecst/ecst/mp/list/type_unwrapper.hpp \
    ecst/ecst/mp/list/types.hpp \
    ecst/ecst/mp/option_map/basic.hpp \
    ecst/ecst/mp/option_map/replace.hpp \
    ecst/ecst/mp/core.hpp \
    ecst/ecst/mp/list.hpp \
    ecst/ecst/mp/option_map.hpp \
    ecst/ecst/settings/data_settings/entity_storage.hpp \
    ecst/ecst/settings/data_settings/multithreading.hpp \
    ecst/ecst/settings/data_settings/refresh.hpp \
    ecst/ecst/settings/data_settings/scheduler.hpp \
    ecst/ecst/settings/impl/keys.hpp \
    ecst/ecst/settings/data.hpp \
    ecst/ecst/settings/data_settings.hpp \
    ecst/ecst/settings/make.hpp \
    ecst/ecst/settings/str.hpp \
    ecst/ecst/signature/component/data.hpp \
    ecst/ecst/signature/component/make.hpp \
    ecst/ecst/signature/component/valid.hpp \
    ecst/ecst/signature/system/impl/keys.hpp \
    ecst/ecst/signature/system/impl/output_wrapper.hpp \
    ecst/ecst/signature/system/data.hpp \
    ecst/ecst/signature/system/dependencies.hpp \
    ecst/ecst/signature/system/make.hpp \
    ecst/ecst/signature/system/tag_of.hpp \
    ecst/ecst/signature/system/valid.hpp \
    ecst/ecst/signature/component.hpp \
    ecst/ecst/signature/system.hpp \
    ecst/ecst/signature_list/component/all_components.hpp \
    ecst/ecst/signature_list/component/id.hpp \
    ecst/ecst/signature_list/component/is_signature_list.hpp \
    ecst/ecst/signature_list/component/make.hpp \
    ecst/ecst/signature_list/component/signature.hpp \
    ecst/ecst/signature_list/system/bf_traversal.hpp \
    ecst/ecst/signature_list/system/dependents.hpp \
    ecst/ecst/signature_list/system/id.hpp \
    ecst/ecst/signature_list/system/id_list.hpp \
    ecst/ecst/signature_list/system/independent.hpp \
    ecst/ecst/signature_list/system/is_signature_list.hpp \
    ecst/ecst/signature_list/system/make.hpp \
    ecst/ecst/signature_list/system/recursive_dependencies.hpp \
    ecst/ecst/signature_list/system/recursive_dependents.hpp \
    ecst/ecst/signature_list/system/signature.hpp \
    ecst/ecst/signature_list/system/signature_list_from_tag_list.hpp \
    ecst/ecst/signature_list/component.hpp \
    ecst/ecst/signature_list/system.hpp \
    ecst/ecst/system_execution_adapter/impl/predicate_holder/predicate_holder.hpp \
    ecst/ecst/system_execution_adapter/impl/predicate_holder.hpp \
    ecst/ecst/system_execution_adapter/impl.hpp \
    ecst/ecst/system_execution_adapter/interface.hpp \
    ecst/ecst/tag/component/tag.hpp \
    ecst/ecst/tag/system/tag.hpp \
    ecst/ecst/tag/component.hpp \
    ecst/ecst/tag/system.hpp \
    ecst/ecst/thread_pool/impl/consumer_queue_ptr.hpp \
    ecst/ecst/thread_pool/impl/pool.hpp \
    ecst/ecst/thread_pool/impl/types.hpp \
    ecst/ecst/thread_pool/impl/worker.hpp \
    ecst/ecst/thread_pool/third_party/concurrent_queue/blockingconcurrentqueue.h \
    ecst/ecst/thread_pool/third_party/concurrent_queue/concurrentqueue.h \
    ecst/ecst/thread_pool/third_party/fixed_function/fixed_function.hpp \
    ecst/ecst/thread_pool/third_party/concurrent_queue.hpp \
    ecst/ecst/thread_pool/third_party/fixed_function.hpp \
    ecst/ecst/thread_pool/aliases.hpp \
    ecst/ecst/thread_pool/third_party.hpp \
    ecst/ecst/thread_pool/thread_pool.hpp \
    ecst/ecst/utils/cv_operations/counter_blocker.hpp \
    ecst/ecst/utils/cv_operations/cv_operations.hpp \
    ecst/ecst/utils/cv_operations.hpp \
    ecst/ecst/utils/do_nothing.hpp \
    ecst/ecst/utils/macros.hpp \
    ecst/ecst/utils/movable_atomic.hpp \
    ecst/ecst/utils/scope_guard.hpp \
    ecst/ecst/utils/sparse_int_set.hpp \
    ecst/ecst/utils/specialized_names.hpp \
    ecst/ecst/aliases.hpp \
    ecst/ecst/config.hpp \
    ecst/ecst/context.hpp \
    ecst/ecst/debug.hpp \
    ecst/ecst/hardware.hpp \
    ecst/ecst/inner_parallelism.hpp \
    ecst/ecst/make_inl.hpp \
    ecst/ecst/mp.hpp \
    ecst/ecst/settings.hpp \
    ecst/ecst/signature.hpp \
    ecst/ecst/signature_list.hpp \
    ecst/ecst/system_execution_adapter.hpp \
    ecst/ecst/tag.hpp \
    ecst/ecst/thread_pool.hpp \
    ecst/ecst/utils.hpp \
    ecst/ecst.hpp \
    mtg/mtg.hpp \
    vrm_core/vrm/core/args_utils/args_slice/args_slice.hpp \
    vrm_core/vrm/core/args_utils/args_slice_aliases/args_slice_aliases.hpp \
    vrm_core/vrm/core/args_utils/impl/wrappers.hpp \
    vrm_core/vrm/core/args_utils/args_slice.hpp \
    vrm_core/vrm/core/args_utils/args_slice_aliases.hpp \
    vrm_core/vrm/core/args_utils/call_utils.hpp \
    vrm_core/vrm/core/args_utils/nth_arg.hpp \
    vrm_core/vrm/core/assert/impl/assert.hpp \
    vrm_core/vrm/core/assert/impl/assert_macros_disabled.hpp \
    vrm_core/vrm/core/assert/impl/assert_macros_enabled.hpp \
    vrm_core/vrm/core/assert/assert.hpp \
    vrm_core/vrm/core/assert/assert_macros.hpp \
    vrm_core/vrm/core/assert/assert_macros_interface.hpp \
    vrm_core/vrm/core/assert/static_assert_macros.hpp \
    vrm_core/vrm/core/casts/impl/arithmetic.hpp \
    vrm_core/vrm/core/casts/impl/arithmetic_conversions.hpp \
    vrm_core/vrm/core/casts/impl/overflow_check.hpp \
    vrm_core/vrm/core/casts/arithmetic.hpp \
    vrm_core/vrm/core/casts/enum.hpp \
    vrm_core/vrm/core/casts/polymorphic.hpp \
    vrm_core/vrm/core/casts/self.hpp \
    vrm_core/vrm/core/casts/storage.hpp \
    vrm_core/vrm/core/casts/string_to_num.hpp \
    vrm_core/vrm/core/casts/void_ptr.hpp \
    vrm_core/vrm/core/config/api/api.hpp \
    vrm_core/vrm/core/config/attributes/attributes.hpp \
    vrm_core/vrm/core/config/detection/arch/impl/arch.hpp \
    vrm_core/vrm/core/config/detection/arch/impl/arch_macros.hpp \
    vrm_core/vrm/core/config/detection/arch/arch.hpp \
    vrm_core/vrm/core/config/detection/compiler/impl/compiler.hpp \
    vrm_core/vrm/core/config/detection/compiler/impl/compiler_macros.hpp \
    vrm_core/vrm/core/config/detection/compiler/compiler.hpp \
    vrm_core/vrm/core/config/detection/debug/impl/debug.hpp \
    vrm_core/vrm/core/config/detection/debug/impl/debug_macros.hpp \
    vrm_core/vrm/core/config/detection/debug/debug.hpp \
    vrm_core/vrm/core/config/detection/os/impl/os.hpp \
    vrm_core/vrm/core/config/detection/os/impl/os_macros.hpp \
    vrm_core/vrm/core/config/detection/os/os.hpp \
    vrm_core/vrm/core/config/detection/rtti/impl/rtti.hpp \
    vrm_core/vrm/core/config/detection/rtti/impl/rtti_macros.hpp \
    vrm_core/vrm/core/config/detection/rtti/rtti.hpp \
    vrm_core/vrm/core/config/detection/stdlib/impl/stdlib.hpp \
    vrm_core/vrm/core/config/detection/stdlib/impl/stdlib_macros.hpp \
    vrm_core/vrm/core/config/detection/stdlib/stdlib.hpp \
    vrm_core/vrm/core/config/detection/arch.hpp \
    vrm_core/vrm/core/config/detection/compiler.hpp \
    vrm_core/vrm/core/config/detection/debug.hpp \
    vrm_core/vrm/core/config/detection/os.hpp \
    vrm_core/vrm/core/config/detection/rtti.hpp \
    vrm_core/vrm/core/config/detection/stdlib.hpp \
    vrm_core/vrm/core/config/names/names.hpp \
    vrm_core/vrm/core/config/api.hpp \
    vrm_core/vrm/core/config/attributes.hpp \
    vrm_core/vrm/core/config/detection.hpp \
    vrm_core/vrm/core/config/names.hpp \
    vrm_core/vrm/core/experimental/console/unix/codes.hpp \
    vrm_core/vrm/core/experimental/console/format_state.hpp \
    vrm_core/vrm/core/experimental/console/format_types.hpp \
    vrm_core/vrm/core/experimental/delegate/impl/base_delegate.hpp \
    vrm_core/vrm/core/experimental/delegate/impl/delegate.hpp \
    vrm_core/vrm/core/experimental/delegate/impl/dynamic_delegate.hpp \
    vrm_core/vrm/core/experimental/delegate/base_delegate.hpp \
    vrm_core/vrm/core/experimental/delegate/delegate.hpp \
    vrm_core/vrm/core/experimental/delegate/dynamic_delegate.hpp \
    vrm_core/vrm/core/experimental/delegate/signature_helper.hpp \
    vrm_core/vrm/core/experimental/handle/impl/storage/array/array.hpp \
    vrm_core/vrm/core/experimental/handle/impl/storage/vector/vector.hpp \
    vrm_core/vrm/core/experimental/handle/impl/storage/array.hpp \
    vrm_core/vrm/core/experimental/handle/impl/storage/vector.hpp \
    vrm_core/vrm/core/experimental/handle/impl/aliases.hpp \
    vrm_core/vrm/core/experimental/handle/impl/settings.hpp \
    vrm_core/vrm/core/experimental/handle/impl/storage.hpp \
    vrm_core/vrm/core/experimental/handle/manager/manager.hpp \
    vrm_core/vrm/core/experimental/handle/strategy/custom.hpp \
    vrm_core/vrm/core/experimental/handle/strategy/storage.hpp \
    vrm_core/vrm/core/experimental/handle/manager.hpp \
    vrm_core/vrm/core/experimental/handle/strategy.hpp \
    vrm_core/vrm/core/experimental/handle/types.hpp \
    vrm_core/vrm/core/experimental/handle2/container/owning_packed_array/owning_packed_array.hpp \
    vrm_core/vrm/core/experimental/handle2/container/owning_packed_array.hpp \
    vrm_core/vrm/core/experimental/handle2/container/vector.hpp \
    vrm_core/vrm/core/experimental/handle2/manager/packed_array/packed_array.hpp \
    vrm_core/vrm/core/experimental/handle2/manager/packed_array.hpp \
    vrm_core/vrm/core/experimental/handle2/settings/aliases.hpp \
    vrm_core/vrm/core/experimental/handle2/settings/settings.hpp \
    vrm_core/vrm/core/experimental/handle2/container.hpp \
    vrm_core/vrm/core/experimental/handle2/context.hpp \
    vrm_core/vrm/core/experimental/handle2/manager.hpp \
    vrm_core/vrm/core/experimental/handle2/settings.hpp \
    vrm_core/vrm/core/experimental/multi_vector/multi_vector.hpp \
    vrm_core/vrm/core/experimental/resizable_buffer/impl/resizable_buffer.hpp \
    vrm_core/vrm/core/experimental/resizable_buffer/multi_resizable_buffer.hpp \
    vrm_core/vrm/core/experimental/resizable_buffer/resizable_buffer.hpp \
    vrm_core/vrm/core/experimental/resource/base/base.hpp \
    vrm_core/vrm/core/experimental/resource/behavior/behavior_traits.hpp \
    vrm_core/vrm/core/experimental/resource/fwd/fwd.hpp \
    vrm_core/vrm/core/experimental/resource/shared/metadata/metadata.hpp \
    vrm_core/vrm/core/experimental/resource/shared/ref_counter/ref_counter.hpp \
    vrm_core/vrm/core/experimental/resource/shared/metadata.hpp \
    vrm_core/vrm/core/experimental/resource/shared/policies.hpp \
    vrm_core/vrm/core/experimental/resource/shared/ref_counter.hpp \
    vrm_core/vrm/core/experimental/resource/shared/shared.hpp \
    vrm_core/vrm/core/experimental/resource/unique/unique.hpp \
    vrm_core/vrm/core/experimental/resource/utils/make_resource.hpp \
    vrm_core/vrm/core/experimental/resource/weak/weak.hpp \
    vrm_core/vrm/core/experimental/resource/access.hpp \
    vrm_core/vrm/core/experimental/resource/base.hpp \
    vrm_core/vrm/core/experimental/resource/behavior.hpp \
    vrm_core/vrm/core/experimental/resource/fwd.hpp \
    vrm_core/vrm/core/experimental/resource/interface.hpp \
    vrm_core/vrm/core/experimental/resource/shared.hpp \
    vrm_core/vrm/core/experimental/resource/unique.hpp \
    vrm_core/vrm/core/experimental/resource/utils.hpp \
    vrm_core/vrm/core/experimental/resource/weak.hpp \
    vrm_core/vrm/core/experimental/sparse_set/impl/storage/dynamic_vector.hpp \
    vrm_core/vrm/core/experimental/sparse_set/impl/storage/fixed_array.hpp \
    vrm_core/vrm/core/experimental/sparse_set/impl/storage/fixed_vector.hpp \
    vrm_core/vrm/core/experimental/sparse_set/impl/storage/shared.hpp \
    vrm_core/vrm/core/experimental/sparse_set/impl/base_sparse_set.hpp \
    vrm_core/vrm/core/experimental/sparse_set/impl/settings.hpp \
    vrm_core/vrm/core/experimental/sparse_set/impl/storage.hpp \
    vrm_core/vrm/core/experimental/sparse_set/base_sparse_set.hpp \
    vrm_core/vrm/core/experimental/sparse_set/sparse_set.hpp \
    vrm_core/vrm/core/experimental/console.hpp \
    vrm_core/vrm/core/experimental/delegate.hpp \
    vrm_core/vrm/core/experimental/handle.hpp \
    vrm_core/vrm/core/experimental/handle2.hpp \
    vrm_core/vrm/core/experimental/multi_vector.hpp \
    vrm_core/vrm/core/experimental/resizable_buffer.hpp \
    vrm_core/vrm/core/experimental/resource.hpp \
    vrm_core/vrm/core/experimental/sparse_set.hpp \
    vrm_core/vrm/core/for_args/for_args.hpp \
    vrm_core/vrm/core/for_args/for_args_complete.hpp \
    vrm_core/vrm/core/for_args/for_args_data.hpp \
    vrm_core/vrm/core/is_valid/is_valid.hpp \
    vrm_core/vrm/core/literals/llong_constant.hpp \
    vrm_core/vrm/core/make_array/make_array.hpp \
    vrm_core/vrm/core/ostream_utils/impl/ignore_manipulator.hpp \
    vrm_core/vrm/core/ostream_utils/ignore_manipulator.hpp \
    vrm_core/vrm/core/ostream_utils/nullptr_printer.hpp \
    vrm_core/vrm/core/overload/make_overload.hpp \
    vrm_core/vrm/core/static_if/impl/static_if_false.hpp \
    vrm_core/vrm/core/static_if/impl/static_if_impl.hpp \
    vrm_core/vrm/core/static_if/impl/static_if_result.hpp \
    vrm_core/vrm/core/static_if/impl/static_if_true.hpp \
    vrm_core/vrm/core/static_if/static_if.hpp \
    vrm_core/vrm/core/strong_typedef/is_strong_typedef.hpp \
    vrm_core/vrm/core/strong_typedef/strong_typedef.hpp \
    vrm_core/vrm/core/strong_typedef/underlying.hpp \
    vrm_core/vrm/core/tuple_utils/impl/tuple_transposer.hpp \
    vrm_core/vrm/core/tuple_utils/apply.hpp \
    vrm_core/vrm/core/tuple_utils/for.hpp \
    vrm_core/vrm/core/tuple_utils/index_of.hpp \
    vrm_core/vrm/core/tuple_utils/ref_tuple.hpp \
    vrm_core/vrm/core/tuple_utils/repeated_tuple.hpp \
    vrm_core/vrm/core/tuple_utils/transpose.hpp \
    vrm_core/vrm/core/type_aliases/integral_constant.hpp \
    vrm_core/vrm/core/type_aliases/numerical.hpp \
    vrm_core/vrm/core/type_traits/common.hpp \
    vrm_core/vrm/core/type_traits/copy_if_rvalue.hpp \
    vrm_core/vrm/core/type_traits/forward_like.hpp \
    vrm_core/vrm/core/type_traits/is_specialization_of.hpp \
    vrm_core/vrm/core/type_traits/nothrow.hpp \
    vrm_core/vrm/core/type_traits/qualifiers.hpp \
    vrm_core/vrm/core/type_traits/referenceness.hpp \
    vrm_core/vrm/core/type_traits/remove_rvalue_reference.hpp \
    vrm_core/vrm/core/type_traits/tuple.hpp \
    vrm_core/vrm/core/utility_macros/impl/noexcept.hpp \
    vrm_core/vrm/core/utility_macros/fwd.hpp \
    vrm_core/vrm/core/utility_macros/likely_unlikely.hpp \
    vrm_core/vrm/core/utility_macros/unreachable.hpp \
    vrm_core/vrm/core/variadic_min_max/variadic_max.hpp \
    vrm_core/vrm/core/variadic_min_max/variadic_min.hpp \
    vrm_core/vrm/core/y_combinator/y_combinator.hpp \
    vrm_core/vrm/core/args_utils.hpp \
    vrm_core/vrm/core/assert.hpp \
    vrm_core/vrm/core/casts.hpp \
    vrm_core/vrm/core/config.hpp \
    vrm_core/vrm/core/experimental.hpp \
    vrm_core/vrm/core/for_args.hpp \
    vrm_core/vrm/core/is_valid.hpp \
    vrm_core/vrm/core/literals.hpp \
    vrm_core/vrm/core/make_array.hpp \
    vrm_core/vrm/core/ostream_utils.hpp \
    vrm_core/vrm/core/overload.hpp \
    vrm_core/vrm/core/static_if.hpp \
    vrm_core/vrm/core/strong_typedef.hpp \
    vrm_core/vrm/core/tuple_utils.hpp \
    vrm_core/vrm/core/type_aliases.hpp \
    vrm_core/vrm/core/type_traits.hpp \
    vrm_core/vrm/core/utility_macros.hpp \
    vrm_core/vrm/core/variadic_min_max.hpp \
    vrm_core/vrm/core/y_combinator.hpp \
    vrm_core/vrm/core.hpp \
    vrm_pp/vrm/pp/generated/output.hpp \
    vrm_pp/vrm/pp/arg_count.hpp \
    vrm_pp/vrm/pp/arithmetic.hpp \
    vrm_pp/vrm/pp/bool.hpp \
    vrm_pp/vrm/pp/cat.hpp \
    vrm_pp/vrm/pp/eval.hpp \
    vrm_pp/vrm/pp/for_each.hpp \
    vrm_pp/vrm/pp/generated.hpp \
    vrm_pp/vrm/pp/repeat.hpp \
    vrm_pp/vrm/pp/sep.hpp \
    vrm_pp/vrm/pp/sep_to_str.hpp \
    vrm_pp/vrm/pp/tpl.hpp \
    vrm_pp/vrm/pp/utils.hpp \
    vrm_pp/vrm/pp.hpp
