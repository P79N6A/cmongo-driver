#include <bson.h>
#include <mongoc.h>
#include <stdio.h>
#include <string>
//int print_keys(mongoc_cursor_t* cursor, const std::string& provid) {
int print_keys(mongoc_cursor_t* cursor) {
    const bson_t* doc;
    bson_iter_t iter;
    bson_iter_t sub_iter;
    bson_iter_t inner_iter;
    bson_iter_t value_iter;
    bson_iter_t city_iter;
    bson_type_t type;
    uint32_t len;
    const bson_value_t *value;

    while (mongoc_cursor_next (cursor, &doc)) {
        if (bson_iter_init (&iter, doc) && (bson_iter_find_descendant (&iter, "province_map.110000", &value_iter))) {
            printf ("Found field name key: %s\n", bson_iter_key (&value_iter));
            type = bson_iter_type (&value_iter);
            printf("The type: %d\n", (int)type);
            bson_iter_recurse(&value_iter, &sub_iter);
            while (bson_iter_next (&sub_iter)) {
                printf ("Found sub-key of \"province_map\" named \"%s\"\n", bson_iter_key (&sub_iter));
                        bson_iter_recurse(&sub_iter, &inner_iter);
                        while (bson_iter_next(&inner_iter)) {
                            printf ("Found sub-key of \"city_ids\" named \"%s\" \n",
                                bson_iter_key (&inner_iter));
                            bson_iter_recurse(&inner_iter, &city_iter);
                            while (bson_iter_next(&city_iter)) {
                                if (strcmp(bson_iter_key(&city_iter), "cityname") == 0) {
                                    printf ("Found sub-key-value of \"%s\" named \"%s\" %s\n",
                                        bson_iter_key (&inner_iter), bson_iter_key (&city_iter), bson_iter_utf8 (&city_iter, &len));
                                } else if (strcmp(bson_iter_key(&city_iter), "spot_num") == 0) {
                                    printf ("Found sub-key-value of \"%s\" named \"%s\" %ld\n",
                                        bson_iter_key (&inner_iter), bson_iter_key (&city_iter), bson_iter_int64 (&city_iter));
                                }
                            }
                        }
                        //printf ("Found sub-key-value of \"spot_ids\" named \"%s\" %s\n",
                        //bson_iter_key (&sub_iter), bson_iter_utf8 (&sub_iter, &len));
                        //citymap.insert(std::make_pair(bson_iter_key (&sub_iter), bson_iter_utf8 (&sub_iter, &len)));
                        //namevec.push_back(bson_iter_utf8(&sub_iter, &len));
            }
            //value = bson_iter_value (&iter);
            //printf("String value is: %s\n", value->value.v_utf8.str);
        }
        if (bson_iter_init (&iter, doc) && (bson_iter_find_descendant (&iter, "province_map.120000", &value_iter))) {
        //if ((bson_iter_find_descendant (&iter, "province_map.120000", &value_iter))) {//必须首先初始化迭代器，否则是查找不到数据的
            printf ("snd time Found field name key: %s\n", bson_iter_key (&value_iter));
            type = bson_iter_type (&value_iter);
            printf("The type: %d\n", (int)type);
            bson_iter_recurse(&value_iter, &sub_iter);
            while (bson_iter_next (&sub_iter)) {
                printf ("Found sub-key of \"province_map\" named \"%s\"\n", bson_iter_key (&sub_iter));
                        bson_iter_recurse(&sub_iter, &inner_iter);
                        while (bson_iter_next(&inner_iter)) {
                            printf ("Found sub-key of \"city_ids\" named \"%s\" \n",
                                bson_iter_key (&inner_iter));
                            bson_iter_recurse(&inner_iter, &city_iter);
                            while (bson_iter_next(&city_iter)) {
                                if (strcmp(bson_iter_key(&city_iter), "cityname") == 0) {
                                    printf ("Found sub-key-value of \"%s\" named \"%s\" %s\n",
                                        bson_iter_key (&inner_iter), bson_iter_key (&city_iter), bson_iter_utf8 (&city_iter, &len));
                                } else if (strcmp(bson_iter_key(&city_iter), "spot_num") == 0) {
                                    printf ("Found sub-key-value of \"%s\" named \"%s\" %ld\n",
                                        bson_iter_key (&inner_iter), bson_iter_key (&city_iter), bson_iter_int64 (&city_iter));
                                }
                            }
                        }
                        //printf ("Found sub-key-value of \"spot_ids\" named \"%s\" %s\n",
                        //bson_iter_key (&sub_iter), bson_iter_utf8 (&sub_iter, &len));
                        //citymap.insert(std::make_pair(bson_iter_key (&sub_iter), bson_iter_utf8 (&sub_iter, &len)));
                        //namevec.push_back(bson_iter_utf8(&sub_iter, &len));
            }
            //value = bson_iter_value (&iter);
            //printf("String value is: %s\n", value->value.v_utf8.str);
        }
    }
}

int main (int argc, char *argv[])
{
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   mongoc_cursor_t *cursor;
   const bson_t *doc;
   bson_t *query;
   char *str;

   mongoc_init ();

   client = mongoc_client_new("mongodb://yq01-wood.epc.baidu.com:8344");
   collection = mongoc_client_get_collection (client, "logistics", "flattable");
   query = bson_new ();
   //query = BSON_INITIALIZER;
   //BSON_APPEND_UTF8 (query, "table_name", "eastchina");
   BSON_APPEND_UTF8 (query, "owner", "zhaotianqi03");

   cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);

/*   while (mongoc_cursor_next (cursor, &doc)) {
      str = bson_as_json (doc, NULL);
      printf ("%s\n", str);
      bson_free (str);
   }*/
   std::string provid("110000");
   print_keys(cursor);

   bson_destroy (query);
   mongoc_cursor_destroy (cursor);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);
   mongoc_cleanup ();

   return 0;
}
