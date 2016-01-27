#include "driver_gattc.h"
#include "ble_err.h"

#include "driver.h"
#include "driver_gatt.h"

static name_map_t gattc_svcs_type_map = {
    NAME_MAP_ENTRY(SD_BLE_GATTC_PRIMARY_SERVICES_DISCOVER),
    NAME_MAP_ENTRY(SD_BLE_GATTC_RELATIONSHIPS_DISCOVER),
    NAME_MAP_ENTRY(SD_BLE_GATTC_CHARACTERISTICS_DISCOVER),
    NAME_MAP_ENTRY(SD_BLE_GATTC_DESCRIPTORS_DISCOVER),
    NAME_MAP_ENTRY(SD_BLE_GATTC_CHAR_VALUE_BY_UUID_READ),
    NAME_MAP_ENTRY(SD_BLE_GATTC_READ),
    NAME_MAP_ENTRY(SD_BLE_GATTC_CHAR_VALUES_READ),
    NAME_MAP_ENTRY(SD_BLE_GATTC_WRITE),
    NAME_MAP_ENTRY(SD_BLE_GATTC_HV_CONFIRM)
};

static name_map_t gattc_evts_type_map = {
    NAME_MAP_ENTRY(BLE_GATTC_EVT_PRIM_SRVC_DISC_RSP),
    NAME_MAP_ENTRY(BLE_GATTC_EVT_REL_DISC_RSP),
    NAME_MAP_ENTRY(BLE_GATTC_EVT_CHAR_DISC_RSP),
    NAME_MAP_ENTRY(BLE_GATTC_EVT_DESC_DISC_RSP),
    NAME_MAP_ENTRY(BLE_GATTC_EVT_CHAR_VAL_BY_UUID_READ_RSP),
    NAME_MAP_ENTRY(BLE_GATTC_EVT_READ_RSP),
    NAME_MAP_ENTRY(BLE_GATTC_EVT_CHAR_VALS_READ_RSP),
    NAME_MAP_ENTRY(BLE_GATTC_EVT_WRITE_RSP),
    NAME_MAP_ENTRY(BLE_GATTC_EVT_HVX),
    NAME_MAP_ENTRY(BLE_GATTC_EVT_TIMEOUT)
};

//
// GattcHandleRange -- START --
//

v8::Local<v8::Object> GattcHandleRange::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    Utility::Set(obj, "start_handle", native->start_handle);
    Utility::Set(obj, "end_handle", native->end_handle);

    return scope.Escape(obj);
}

ble_gattc_handle_range_t *GattcHandleRange::ToNative()
{
    if (Utility::IsNull(jsobj))
    {
        return 0;
    }

    ble_gattc_handle_range_t *handleRange = new ble_gattc_handle_range_t();

    handleRange->start_handle = ConversionUtility::getNativeUint16(jsobj, "start_handle");
    handleRange->end_handle = ConversionUtility::getNativeUint16(jsobj, "end_handle");

    return handleRange;
}

//
// GattcHandleRange -- END --
//

//
// GattcService -- START --
//

v8::Local<v8::Object> GattcService::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    Utility::Set(obj, "uuid", BleUUID(&native->uuid).ToJs());
    Utility::Set(obj, "handle_range", GattcHandleRange(&native->handle_range).ToJs());

    return scope.Escape(obj);
}

//
// GattcService -- END --
//

//
// GattcIncludedService -- START --
//

v8::Local<v8::Object> GattcIncludedService::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    Utility::Set(obj, "handle", native->handle);
    Utility::Set(obj, "included_srvc", GattcService(&native->included_srvc).ToJs());

    return scope.Escape(obj);
}

//
// GattcIncludedService -- END --
//

//
// GattcCharacteristic -- START --
//

v8::Local<v8::Object> GattcCharacteristic::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    Utility::Set(obj, "uuid", BleUUID(&native->uuid).ToJs());
    Utility::Set(obj, "char_props", GattCharProps(&native->char_props).ToJs());
    Utility::Set(obj, "char_ext_props", native->char_ext_props);
    Utility::Set(obj, "handle_decl", native->handle_decl);
    Utility::Set(obj, "handle_value", native->handle_value);

    return scope.Escape(obj);
}

//
// GattcCharacteristic -- END --
//

//
// GattcDescriptor -- START --
//

v8::Local<v8::Object> GattcDescriptor::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    Utility::Set(obj, "handle", native->handle);
    Utility::Set(obj, "uuid", BleUUID(&native->uuid).ToJs());

    return scope.Escape(obj);
}

//
// GattcDescriptor -- END --
//

//
// GattcWriteParameters -- START --
//

ble_gattc_write_params_t *GattcWriteParameters::ToNative()
{
    if (Utility::IsNull(jsobj))
    {
        return 0;
    }

    ble_gattc_write_params_t *writeparams = new ble_gattc_write_params_t();

    writeparams->write_op = ConversionUtility::getNativeUint8(jsobj, "write_op");
    writeparams->flags = ConversionUtility::getNativeUint8(jsobj, "flags");
    writeparams->handle = ConversionUtility::getNativeUint16(jsobj, "handle");
    writeparams->offset = ConversionUtility::getNativeUint16(jsobj, "offset");
    writeparams->len = ConversionUtility::getNativeUint16(jsobj, "len");
    writeparams->p_value = ConversionUtility::getNativePointerToUint8(jsobj, "value");

    return writeparams;
}

v8::Local<v8::Object> GattcWriteParameters::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    Utility::Set(obj, "write_op", native->write_op);
    Utility::Set(obj, "flags", native->flags);
    Utility::Set(obj, "handle", native->handle);
    Utility::Set(obj, "offset", native->offset);
    Utility::Set(obj, "len", native->len);
    Utility::Set(obj, "value", ConversionUtility::toJsValueArray(native->p_value, native->len));

    return scope.Escape(obj);
}

//
// GattcWriteParameters -- END --
//

v8::Local<v8::Object> GattcPrimaryServiceDiscoveryEvent::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    BleDriverGattcEvent::ToJs(obj);

    Utility::Set(obj, "count", evt->count);

    v8::Local<v8::Array> service_array = Nan::New<v8::Array>();

    for (int i = 0; i < evt->count; ++i)
    {
        service_array->Set(Nan::New<v8::Integer>(i), GattcService(&evt->services[i]));
    }

    Utility::Set(obj, "services", service_array);

    return scope.Escape(obj);
}

v8::Local<v8::Object> GattcRelationshipDiscoveryEvent::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    BleDriverGattcEvent::ToJs(obj);

    Utility::Set(obj, "count", evt->count);

    v8::Local<v8::Array> includes_array = Nan::New<v8::Array>();

    for (int i = 0; i < evt->count; ++i)
    {
        includes_array->Set(Nan::New<v8::Integer>(i), GattcIncludedService(&evt->includes[i]));
    }

    Utility::Set(obj, "includes", includes_array);

    return scope.Escape(obj);
}

v8::Local<v8::Object> GattcCharacteristicDiscoveryEvent::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    BleDriverGattcEvent::ToJs(obj);

    Utility::Set(obj, "count", evt->count);

    v8::Local<v8::Array> chars_array = Nan::New<v8::Array>();

    for (int i = 0; i < evt->count; ++i)
    {
        chars_array->Set(Nan::New<v8::Integer>(i), GattcCharacteristic(&evt->chars[i]));
    }

    Utility::Set(obj, "chars", chars_array);

    return scope.Escape(obj);
}

v8::Local<v8::Object> GattcDescriptorDiscoveryEvent::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    BleDriverGattcEvent::ToJs(obj);

    Utility::Set(obj, "count", evt->count);

    v8::Local<v8::Array> descs_array = Nan::New<v8::Array>();

    for (int i = 0; i < evt->count; ++i)
    {
        descs_array->Set(Nan::New<v8::Integer>(i), GattcDescriptor(&evt->descs[i]));
    }

    Utility::Set(obj, "descs", descs_array);

    return scope.Escape(obj);
}

v8::Local<v8::Object> GattcHandleValue::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    Utility::Set(obj, "handle", native->handle);
    Utility::Set(obj, "value", ConversionUtility::toJsValueArray(native->p_value, valueLength));

    return scope.Escape(obj);
}

v8::Local<v8::Object> GattcCharacteristicValueReadByUUIDEvent::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    BleDriverGattcEvent::ToJs(obj);

    Utility::Set(obj, "count", evt->count);
    Utility::Set(obj, "value_len", evt->value_len);

    v8::Local<v8::Array> handle_value_array = Nan::New<v8::Array>();

    for (int i = 0; i < evt->count; ++i)
    {
        handle_value_array->Set(Nan::New<v8::Integer>(i), GattcHandleValue(&evt->handle_value[i], evt->value_len));
    }

    Utility::Set(obj, "handle_values", handle_value_array);

    return scope.Escape(obj);
}

v8::Local<v8::Object> GattcReadEvent::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    BleDriverGattcEvent::ToJs(obj);

    Utility::Set(obj, "handle", evt->handle);
    Utility::Set(obj, "offset", evt->offset);
    Utility::Set(obj, "len", evt->len);
    Utility::Set(obj, "data", ConversionUtility::toJsValueArray(evt->data, evt->len));

    return scope.Escape(obj);
}

v8::Local<v8::Object> GattcCharacteristicValueReadEvent::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    BleDriverGattcEvent::ToJs(obj);

    Utility::Set(obj, "len", evt->len);
    Utility::Set(obj, "values", ConversionUtility::toJsValueArray(evt->values, evt->len));

    return scope.Escape(obj);
}

v8::Local<v8::Object> GattcWriteEvent::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    BleDriverGattcEvent::ToJs(obj);

    Utility::Set(obj, "handle", evt->handle);
    Utility::Set(obj, "write_op", evt->write_op);
    Utility::Set(obj, "offset", evt->offset);
    Utility::Set(obj, "len", evt->len);
    Utility::Set(obj, "data", ConversionUtility::toJsValueArray(evt->data, evt->len));

    return scope.Escape(obj);
}

v8::Local<v8::Object> GattcHandleValueNotificationEvent::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    BleDriverGattcEvent::ToJs(obj);

    Utility::Set(obj, "handle", evt->handle);
    Utility::Set(obj, "type", evt->type);
    Utility::Set(obj, "len", evt->len);
    Utility::Set(obj, "data", ConversionUtility::toJsValueArray(evt->data, evt->len));

    return scope.Escape(obj);
}

v8::Local<v8::Object> GattcTimeoutEvent::ToJs()
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    BleDriverGattcEvent::ToJs(obj);

    Utility::Set(obj, "src", evt->src);

    return scope.Escape(obj);
}

NAN_METHOD(Adapter::GattcDiscoverPrimaryServices)
{
    Adapter* obj = Nan::ObjectWrap::Unwrap<Adapter>(info.Holder());
    uint16_t conn_handle;
    uint16_t start_handle;
    v8::Local<v8::Object> service_uuid;
    v8::Local<v8::Function> callback;
    int argumentcount = 0;

    try
    {
        conn_handle = ConversionUtility::getNativeUint16(info[argumentcount]);
        argumentcount++;

        start_handle = ConversionUtility::getNativeUint16(info[argumentcount]);
        argumentcount++;

        service_uuid = ConversionUtility::getJsObjectOrNull(info[argumentcount]);
        argumentcount++;

        callback = ConversionUtility::getCallbackFunction(info[argumentcount]);
        argumentcount++;
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getTypeErrorMessage(argumentcount, error);
        Nan::ThrowTypeError(message);
        return;
    }

    GattcDiscoverPrimaryServicesBaton *baton = new GattcDiscoverPrimaryServicesBaton(callback);
    baton->conn_handle = conn_handle;
    baton->start_handle = start_handle;

    try
    {
        baton->p_srvc_uuid = BleUUID(service_uuid);
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getStructErrorMessage("srvc_uuid", error);
        Nan::ThrowTypeError(message);
        return;
    }

    uv_queue_work(uv_default_loop(), baton->req, GattcDiscoverPrimaryServices, (uv_after_work_cb)AfterGattcDiscoverPrimaryServices);
}

// This runs in a worker thread (not Main Thread)
void Adapter::GattcDiscoverPrimaryServices(uv_work_t *req)
{
    GattcDiscoverPrimaryServicesBaton *baton = static_cast<GattcDiscoverPrimaryServicesBaton *>(req->data);

    std::lock_guard<std::mutex> lock(ble_driver_call_mutex);
    baton->result = sd_ble_gattc_primary_services_discover(baton->adapter, baton->conn_handle, baton->start_handle, baton->p_srvc_uuid);
}

// This runs in Main Thread
void Adapter::AfterGattcDiscoverPrimaryServices(uv_work_t *req)
{
	Nan::HandleScope scope;

    GattcDiscoverPrimaryServicesBaton *baton = static_cast<GattcDiscoverPrimaryServicesBaton *>(req->data);
    v8::Local<v8::Value> argv[1];

    if (baton->result != NRF_SUCCESS)
    {
        argv[0] = ErrorMessage::getErrorMessage(baton->result, "starting service discovery");
    }
    else
    {
        argv[0] = Nan::Undefined();
    }

    baton->callback->Call(1, argv);
    delete baton;
}


NAN_METHOD(Adapter::GattcDiscoverRelationship)
{
    Adapter* obj = Nan::ObjectWrap::Unwrap<Adapter>(info.Holder());
    uint16_t conn_handle;
    v8::Local<v8::Object> handle_range;
    v8::Local<v8::Function> callback;
    int argumentcount = 0;

    try
    {
        conn_handle = ConversionUtility::getNativeUint16(info[argumentcount]);
        argumentcount++;

        handle_range = ConversionUtility::getJsObject(info[argumentcount]);
        argumentcount++;

        callback = ConversionUtility::getCallbackFunction(info[argumentcount]);
        argumentcount++;
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getTypeErrorMessage(argumentcount, error);
        Nan::ThrowTypeError(message);
        return;
    }

    GattcDiscoverRelationshipBaton *baton = new GattcDiscoverRelationshipBaton(callback);
    baton->conn_handle = conn_handle;

    try
    {
        baton->p_handle_range = GattcHandleRange(handle_range);
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getStructErrorMessage("handle_range", error);
        Nan::ThrowTypeError(message);
        return;
    }

    uv_queue_work(uv_default_loop(), baton->req, GattcDiscoverRelationship, (uv_after_work_cb)AfterGattcDiscoverRelationship);
}

// This runs in a worker thread (not Main Thread)
void Adapter::GattcDiscoverRelationship(uv_work_t *req)
{
    GattcDiscoverRelationshipBaton *baton = static_cast<GattcDiscoverRelationshipBaton *>(req->data);

    std::lock_guard<std::mutex> lock(ble_driver_call_mutex);
    baton->result = sd_ble_gattc_relationships_discover(baton->adapter, baton->conn_handle, baton->p_handle_range);
}

// This runs in Main Thread
void Adapter::AfterGattcDiscoverRelationship(uv_work_t *req)
{
	Nan::HandleScope scope;

    GattcDiscoverRelationshipBaton *baton = static_cast<GattcDiscoverRelationshipBaton *>(req->data);
    v8::Local<v8::Value> argv[1];

    if (baton->result != NRF_SUCCESS)
    {
        argv[0] = ErrorMessage::getErrorMessage(baton->result, "starting relationship discovery");
    }
    else
    {
        argv[0] = Nan::Undefined();
    }

    baton->callback->Call(1, argv);
    delete baton;
}


NAN_METHOD(Adapter::GattcDiscoverCharacteristics)
{
    Adapter* obj = Nan::ObjectWrap::Unwrap<Adapter>(info.Holder());
    uint16_t conn_handle;
    v8::Local<v8::Object> handle_range;
    v8::Local<v8::Function> callback;
    int argumentcount = 0;

    try
    {
        conn_handle = ConversionUtility::getNativeUint16(info[argumentcount]);
        argumentcount++;

        handle_range = ConversionUtility::getJsObject(info[argumentcount]);
        argumentcount++;

        callback = ConversionUtility::getCallbackFunction(info[argumentcount]);
        argumentcount++;
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getTypeErrorMessage(argumentcount, error);
        Nan::ThrowTypeError(message);
        return;
    }

    GattcDiscoverCharacteristicsBaton *baton = new GattcDiscoverCharacteristicsBaton(callback);
    baton->conn_handle = conn_handle;

    try
    {
        baton->p_handle_range = GattcHandleRange(handle_range);
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getStructErrorMessage("handle_range", error);
        Nan::ThrowTypeError(message);
        return;
    }

    uv_queue_work(uv_default_loop(), baton->req, GattcDiscoverCharacteristics, (uv_after_work_cb)AfterGattcDiscoverCharacteristics);
}

// This runs in a worker thread (not Main Thread)
void Adapter::GattcDiscoverCharacteristics(uv_work_t *req)
{
    GattcDiscoverCharacteristicsBaton *baton = static_cast<GattcDiscoverCharacteristicsBaton *>(req->data);

    std::lock_guard<std::mutex> lock(ble_driver_call_mutex);
    baton->result = sd_ble_gattc_characteristics_discover(baton->adapter, baton->conn_handle, baton->p_handle_range);
}

// This runs in Main Thread
void Adapter::AfterGattcDiscoverCharacteristics(uv_work_t *req)
{
	Nan::HandleScope scope;

    GattcDiscoverCharacteristicsBaton *baton = static_cast<GattcDiscoverCharacteristicsBaton *>(req->data);
    v8::Local<v8::Value> argv[1];

    if (baton->result != NRF_SUCCESS)
    {
        argv[0] = ErrorMessage::getErrorMessage(baton->result, "starting characteristic discovery");
    }
    else
    {
        argv[0] = Nan::Undefined();
    }

    baton->callback->Call(1, argv);
    delete baton;
}


NAN_METHOD(Adapter::GattcDiscoverDescriptors)
{
    Adapter* obj = Nan::ObjectWrap::Unwrap<Adapter>(info.Holder());
    uint16_t conn_handle;
    v8::Local<v8::Object> handle_range;
    v8::Local<v8::Function> callback;
    int argumentcount = 0;

    try
    {
        conn_handle = ConversionUtility::getNativeUint16(info[argumentcount]);
        argumentcount++;

        handle_range = ConversionUtility::getJsObject(info[argumentcount]);
        argumentcount++;

        callback = ConversionUtility::getCallbackFunction(info[argumentcount]);
        argumentcount++;
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getTypeErrorMessage(argumentcount, error);
        Nan::ThrowTypeError(message);
        return;
    }

    GattcDiscoverDescriptorsBaton *baton = new GattcDiscoverDescriptorsBaton(callback);
    baton->conn_handle = conn_handle;

    try
    {
        baton->p_handle_range = GattcHandleRange(handle_range);
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getStructErrorMessage("handle_range", error);
        Nan::ThrowTypeError(message);
        return;
    }

    uv_queue_work(uv_default_loop(), baton->req, GattcDiscoverDescriptors, (uv_after_work_cb)AfterGattcDiscoverDescriptors);
}

// This runs in a worker thread (not Main Thread)
void Adapter::GattcDiscoverDescriptors(uv_work_t *req)
{
    GattcDiscoverDescriptorsBaton *baton = static_cast<GattcDiscoverDescriptorsBaton *>(req->data);

    std::lock_guard<std::mutex> lock(ble_driver_call_mutex);
    baton->result = sd_ble_gattc_descriptors_discover(baton->adapter, baton->conn_handle, baton->p_handle_range);
}

// This runs in Main Thread
void Adapter::AfterGattcDiscoverDescriptors(uv_work_t *req)
{
	Nan::HandleScope scope;

    GattcDiscoverDescriptorsBaton *baton = static_cast<GattcDiscoverDescriptorsBaton *>(req->data);
    v8::Local<v8::Value> argv[1];

    if (baton->result != NRF_SUCCESS)
    {
        argv[0] = ErrorMessage::getErrorMessage(baton->result, "starting descriptor discovery");
    }
    else
    {
        argv[0] = Nan::Undefined();
    }

    baton->callback->Call(1, argv);
    delete baton;
}


NAN_METHOD(Adapter::GattcReadCharacteristicValueByUUID)
{
    Adapter* obj = Nan::ObjectWrap::Unwrap<Adapter>(info.Holder());
    uint16_t conn_handle;
    v8::Local<v8::Object> uuid;
    v8::Local<v8::Object> handle_range;
    v8::Local<v8::Function> callback;
    int argumentcount = 0;

    try
    {
        conn_handle = ConversionUtility::getNativeUint16(info[argumentcount]);
        argumentcount++;

        uuid = ConversionUtility::getJsObject(info[argumentcount]);
        argumentcount++;

        handle_range = ConversionUtility::getJsObject(info[argumentcount]);
        argumentcount++;

        callback = ConversionUtility::getCallbackFunction(info[argumentcount]);
        argumentcount++;
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getTypeErrorMessage(argumentcount, error);
        Nan::ThrowTypeError(message);
        return;
    }

    GattcCharacteristicByUUIDReadBaton *baton = new GattcCharacteristicByUUIDReadBaton(callback);
    baton->conn_handle = conn_handle;
    try
    {
        baton->p_uuid = BleUUID(uuid);
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getStructErrorMessage("uuid", error);
        Nan::ThrowTypeError(message);
        return;
    }

    try
    {
        baton->p_handle_range = GattcHandleRange(handle_range);
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getStructErrorMessage("handle_range", error);
        Nan::ThrowTypeError(message);
        return;
    }

    uv_queue_work(uv_default_loop(), baton->req, GattcReadCharacteristicValueByUUID, (uv_after_work_cb)AfterGattcReadCharacteristicValueByUUID);
}

// This runs in a worker thread (not Main Thread)
void Adapter::GattcReadCharacteristicValueByUUID(uv_work_t *req)
{
    GattcCharacteristicByUUIDReadBaton *baton = static_cast<GattcCharacteristicByUUIDReadBaton *>(req->data);

    std::lock_guard<std::mutex> lock(ble_driver_call_mutex);
    baton->result = sd_ble_gattc_char_value_by_uuid_read(baton->adapter, baton->conn_handle, baton->p_uuid, baton->p_handle_range);
}

// This runs in Main Thread
void Adapter::AfterGattcReadCharacteristicValueByUUID(uv_work_t *req)
{
	Nan::HandleScope scope;

    GattcCharacteristicByUUIDReadBaton *baton = static_cast<GattcCharacteristicByUUIDReadBaton *>(req->data);
    v8::Local<v8::Value> argv[1];

    if (baton->result != NRF_SUCCESS)
    {
        argv[0] = ErrorMessage::getErrorMessage(baton->result, "starting reading characteristics by UUID");
    }
    else
    {
        argv[0] = Nan::Undefined();
    }

    baton->callback->Call(1, argv);
    delete baton;
}


NAN_METHOD(Adapter::GattcRead)
{
    Adapter* obj = Nan::ObjectWrap::Unwrap<Adapter>(info.Holder());
    uint16_t conn_handle;
    uint16_t handle;
    uint16_t offset;
    v8::Local<v8::Function> callback;
    int argumentcount = 0;

    try
    {
        conn_handle = ConversionUtility::getNativeUint16(info[argumentcount]);
        argumentcount++;

        handle = ConversionUtility::getNativeUint16(info[argumentcount]);
        argumentcount++;

        offset = ConversionUtility::getNativeUint16(info[argumentcount]);
        argumentcount++;

        callback = ConversionUtility::getCallbackFunction(info[argumentcount]);
        argumentcount++;
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getTypeErrorMessage(argumentcount, error);
        Nan::ThrowTypeError(message);
        return;
    }

    GattcReadBaton *baton = new GattcReadBaton(callback);
    baton->conn_handle = conn_handle;
    baton->handle = handle;
    baton->offset = offset;

    uv_queue_work(uv_default_loop(), baton->req, GattcRead, (uv_after_work_cb)AfterGattcRead);
}

// This runs in a worker thread (not Main Thread)
void Adapter::GattcRead(uv_work_t *req)
{
    GattcReadBaton *baton = static_cast<GattcReadBaton *>(req->data);

    std::lock_guard<std::mutex> lock(ble_driver_call_mutex);
    baton->result = sd_ble_gattc_read(baton->adapter, baton->conn_handle, baton->handle, baton->offset);
}

// This runs in Main Thread
void Adapter::AfterGattcRead(uv_work_t *req)
{
	Nan::HandleScope scope;

    GattcReadBaton *baton = static_cast<GattcReadBaton *>(req->data);
    v8::Local<v8::Value> argv[1];

    if (baton->result != NRF_SUCCESS)
    {
        argv[0] = ErrorMessage::getErrorMessage(baton->result, "starting reading");
    }
    else
    {
        argv[0] = Nan::Undefined();
    }

    baton->callback->Call(1, argv);
    delete baton;
}

NAN_METHOD(Adapter::GattcReadCharacteristicValues)
{
    Adapter* obj = Nan::ObjectWrap::Unwrap<Adapter>(info.Holder());
    uint16_t conn_handle;
    v8::Local<v8::Object> handles;
    uint16_t handle_count;
    v8::Local<v8::Function> callback;
    int argumentcount = 0;

    try
    {
        conn_handle = ConversionUtility::getNativeUint16(info[argumentcount]);
        argumentcount++;

        handles = ConversionUtility::getJsObject(info[argumentcount]);
        argumentcount++;

        handle_count = ConversionUtility::getNativeUint16(info[argumentcount]);
        argumentcount++;

        callback = ConversionUtility::getCallbackFunction(info[argumentcount]);
        argumentcount++;
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getTypeErrorMessage(argumentcount, error);
        Nan::ThrowTypeError(message);
        return;
    }

    uint16_t *p_handles = (uint16_t *)malloc(sizeof(uint16_t) * handle_count);

    try
    {
        for (int i = 0; i < handle_count; ++i)
        {
            p_handles[i] = ConversionUtility::getNativeUint16(handles->Get(Nan::New<v8::Number>(i)));
        }
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getStructErrorMessage("handles", error);
        Nan::ThrowTypeError(message);
        return;
    }

    GattcReadCharacteristicValuesBaton *baton = new GattcReadCharacteristicValuesBaton(callback);
    baton->conn_handle = conn_handle;
    baton->p_handles = p_handles;
    baton->handle_count = handle_count;

    uv_queue_work(uv_default_loop(), baton->req, GattcReadCharacteristicValues, (uv_after_work_cb)AfterGattcReadCharacteristicValues);
}

// This runs in a worker thread (not Main Thread)
void Adapter::GattcReadCharacteristicValues(uv_work_t *req)
{
    GattcReadCharacteristicValuesBaton *baton = static_cast<GattcReadCharacteristicValuesBaton *>(req->data);

    std::lock_guard<std::mutex> lock(ble_driver_call_mutex);
    baton->result = sd_ble_gattc_char_values_read(baton->adapter, baton->conn_handle, baton->p_handles, baton->handle_count);

    free(baton->p_handles);
}

// This runs in Main Thread
void Adapter::AfterGattcReadCharacteristicValues(uv_work_t *req)
{
	Nan::HandleScope scope;

    GattcReadCharacteristicValuesBaton *baton = static_cast<GattcReadCharacteristicValuesBaton *>(req->data);
    v8::Local<v8::Value> argv[1];

    if (baton->result != NRF_SUCCESS)
    {
        argv[0] = ErrorMessage::getErrorMessage(baton->result, "starting reading characteristics values");
    }
    else
    {
        argv[0] = Nan::Undefined();
    }

    baton->callback->Call(1, argv);
    delete baton;
}


NAN_METHOD(Adapter::GattcWrite)
{
    Adapter* obj = Nan::ObjectWrap::Unwrap<Adapter>(info.Holder());
    uint16_t conn_handle;
    v8::Local<v8::Object> p_write_params;
    v8::Local<v8::Function> callback;
    int argumentcount = 0;

    try
    {
        conn_handle = ConversionUtility::getNativeUint16(info[argumentcount]);
        argumentcount++;

        p_write_params = ConversionUtility::getJsObject(info[argumentcount]);
        argumentcount++;

        callback = ConversionUtility::getCallbackFunction(info[argumentcount]);
        argumentcount++;
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getTypeErrorMessage(argumentcount, error);
        Nan::ThrowTypeError(message);
        return;
    }

    GattcWriteBaton *baton = new GattcWriteBaton(callback);
    baton->conn_handle = conn_handle;

    try
    {
        baton->p_write_params = GattcWriteParameters(p_write_params);
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getStructErrorMessage("write_params", error);
        Nan::ThrowTypeError(message);
        return;
    }

    uv_queue_work(uv_default_loop(), baton->req, GattcWrite, (uv_after_work_cb)AfterGattcWrite);
}

// This runs in a worker thread (not Main Thread)
void Adapter::GattcWrite(uv_work_t *req)
{
    GattcWriteBaton *baton = static_cast<GattcWriteBaton *>(req->data);

    std::lock_guard<std::mutex> lock(ble_driver_call_mutex);
    baton->result = sd_ble_gattc_write(baton->adapter, baton->conn_handle, baton->p_write_params);
}

// This runs in Main Thread
void Adapter::AfterGattcWrite(uv_work_t *req)
{
	Nan::HandleScope scope;

    GattcWriteBaton *baton = static_cast<GattcWriteBaton *>(req->data);
    v8::Local<v8::Value> argv[1];

    if (baton->result != NRF_SUCCESS)
    {
        argv[0] = ErrorMessage::getErrorMessage(baton->result, "writing");
    }
    else
    {
        argv[0] = Nan::Undefined();
    }

    baton->callback->Call(1, argv);
    delete baton;
}

NAN_METHOD(Adapter::GattcConfirmHandleValue)
{
    Adapter* obj = Nan::ObjectWrap::Unwrap<Adapter>(info.Holder());
    uint16_t conn_handle;
    uint16_t handle;
    v8::Local<v8::Function> callback;
    int argumentcount = 0;

    try
    {
        conn_handle = ConversionUtility::getNativeUint16(info[argumentcount]);
        argumentcount++;

        handle = ConversionUtility::getNativeUint16(info[argumentcount]);
        argumentcount++;

        callback = ConversionUtility::getCallbackFunction(info[argumentcount]);
        argumentcount++;
    }
    catch (char const *error)
    {
        v8::Local<v8::String> message = ErrorMessage::getTypeErrorMessage(argumentcount, error);
        Nan::ThrowTypeError(message);
        return;
    }

    GattcConfirmHandleValueBaton *baton = new GattcConfirmHandleValueBaton(callback);
    baton->conn_handle = conn_handle;
    baton->handle = handle;

    uv_queue_work(uv_default_loop(), baton->req, GattcConfirmHandleValue, (uv_after_work_cb)AfterGattcConfirmHandleValue);
}

// This runs in a worker thread (not Main Thread)
void Adapter::GattcConfirmHandleValue(uv_work_t *req)
{
    GattcConfirmHandleValueBaton *baton = static_cast<GattcConfirmHandleValueBaton *>(req->data);

    std::lock_guard<std::mutex> lock(ble_driver_call_mutex);
    baton->result = sd_ble_gattc_hv_confirm(baton->adapter, baton->conn_handle, baton->handle);
}

// This runs in Main Thread
void Adapter::AfterGattcConfirmHandleValue(uv_work_t *req)
{
	Nan::HandleScope scope;

    GattcConfirmHandleValueBaton *baton = static_cast<GattcConfirmHandleValueBaton *>(req->data);
    v8::Local<v8::Value> argv[1];

    if (baton->result != NRF_SUCCESS)
    {
        argv[0] = ErrorMessage::getErrorMessage(baton->result, "confirming handle value");
    }
    else
    {
        argv[0] = Nan::Undefined();
    }

    baton->callback->Call(1, argv);
    delete baton;
}

extern "C" {
    void init_gattc(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target)
    {
        /* BLE_ERRORS_GATTC SVC return values specific to GATTC */
        NODE_DEFINE_CONSTANT(target, BLE_ERROR_GATTC_PROC_NOT_PERMITTED);

        /* Last Attribute Handle. */
        NODE_DEFINE_CONSTANT(target, BLE_GATTC_HANDLE_END);

        NODE_DEFINE_CONSTANT(target, SD_BLE_GATTC_PRIMARY_SERVICES_DISCOVER);                      /**< Primary Service Discovery. */
        NODE_DEFINE_CONSTANT(target, SD_BLE_GATTC_RELATIONSHIPS_DISCOVER);                         /**< Relationship Discovery. */
        NODE_DEFINE_CONSTANT(target, SD_BLE_GATTC_CHARACTERISTICS_DISCOVER);                       /**< Characteristic Discovery. */
        NODE_DEFINE_CONSTANT(target, SD_BLE_GATTC_DESCRIPTORS_DISCOVER);                           /**< Characteristic Descriptor Discovery. */
        NODE_DEFINE_CONSTANT(target, SD_BLE_GATTC_CHAR_VALUE_BY_UUID_READ);                        /**< Read Characteristic Value by UUID. */
        NODE_DEFINE_CONSTANT(target, SD_BLE_GATTC_READ);                                           /**< Generic read. */
        NODE_DEFINE_CONSTANT(target, SD_BLE_GATTC_CHAR_VALUES_READ);                               /**< Read multiple Characteristic Values. */
        NODE_DEFINE_CONSTANT(target, SD_BLE_GATTC_WRITE);                                          /**< Generic write. */
        NODE_DEFINE_CONSTANT(target, SD_BLE_GATTC_HV_CONFIRM);                                     /**< Handle Value Confirmation. */

        NODE_DEFINE_CONSTANT(target, BLE_GATTC_EVT_PRIM_SRVC_DISC_RSP);                       /**< Primary Service Discovery Response event. @ref ble_gattc_evt_prim_srvc_disc_rsp_t */
        NODE_DEFINE_CONSTANT(target, BLE_GATTC_EVT_REL_DISC_RSP);                             /**< Relationship Discovery Response event. @ref ble_gattc_evt_rel_disc_rsp_t */
        NODE_DEFINE_CONSTANT(target, BLE_GATTC_EVT_CHAR_DISC_RSP);                            /**< Characteristic Discovery Response event. @ref ble_gattc_evt_char_disc_rsp_t */
        NODE_DEFINE_CONSTANT(target, BLE_GATTC_EVT_DESC_DISC_RSP);                            /**< Descriptor Discovery Response event. @ref ble_gattc_evt_desc_disc_rsp_t */
        NODE_DEFINE_CONSTANT(target, BLE_GATTC_EVT_CHAR_VAL_BY_UUID_READ_RSP);                /**< Read By UUID Response event. @ref ble_gattc_evt_char_val_by_uuid_read_rsp_t */
        NODE_DEFINE_CONSTANT(target, BLE_GATTC_EVT_READ_RSP);                                 /**< Read Response event. @ref ble_gattc_evt_read_rsp_t */
        NODE_DEFINE_CONSTANT(target, BLE_GATTC_EVT_CHAR_VALS_READ_RSP);                       /**< Read multiple Response event. @ref ble_gattc_evt_char_vals_read_rsp_t */
        NODE_DEFINE_CONSTANT(target, BLE_GATTC_EVT_WRITE_RSP);                                /**< Write Response event. @ref ble_gattc_evt_write_rsp_t */
        NODE_DEFINE_CONSTANT(target, BLE_GATTC_EVT_HVX);                                      /**< Handle Value Notification or Indication event. @ref ble_gattc_evt_hvx_t */
        NODE_DEFINE_CONSTANT(target, BLE_GATTC_EVT_TIMEOUT);                                  /**< Timeout event. @ref ble_gattc_evt_timeout_t */
    }
}
