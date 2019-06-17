#include <bson.h>
#include <mongoc.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    bson_error_t error;

    mongoc_init ();

    client = mongoc_client_new("mongodb://yq01-wood.epc.baidu.com:8344");
    collection = mongoc_client_get_collection (client, "logistics", "testcrud");
    const char* array_key;
    char array_key_buf[32];
    uint32_t array_index = 0;
    char* json;

    bson_t* doc;
    doc = bson_new ();
    bson_oid_t oid;
    bson_t content, province_map, yiyiling, spot_list, city_ids;
    bson_oid_init (&oid, NULL);
    BSON_APPEND_OID (doc, "table_id", &oid);
    BSON_APPEND_UTF8(doc, "table_name", "eastchina");
    BSON_APPEND_DOCUMENT_BEGIN(doc, "content", &content);
    BSON_APPEND_DOCUMENT_BEGIN(&content, "province_map", &province_map);
    BSON_APPEND_DOCUMENT_BEGIN(&province_map, "yiyiling", &yiyiling);
    BSON_APPEND_INT32(&yiyiling, "spot_num", 65);
    BSON_APPEND_ARRAY_BEGIN(&yiyiling, "city_ids", &city_ids);
    bson_uint32_to_string(
        array_index++, &array_key, array_key_buf, sizeof array_key_buf);
    BSON_APPEND_INT32(&city_ids, array_key, 111010);//接着往数组元素里插入元素site_detail，元素key未知
    bson_append_array_end(&yiyiling, &city_ids);
    bson_append_document_end(&province_map, &yiyiling);
    bson_append_document_end(&content, &province_map);
    bson_append_document_end(doc, &content);

    if (!mongoc_collection_insert (collection, MONGOC_INSERT_NONE, doc, NULL, &error)) {
        fprintf (stderr, "%s\n", error.message);
    }
    json = bson_as_json(doc, NULL);
    printf("%s\n", json);
    free(json);
    bson_destroy(doc);
    return 0;
}
