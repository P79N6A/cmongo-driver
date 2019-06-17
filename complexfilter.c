#include <bson.h>
#include <mongoc.h>
#include <stdio.h>
//#include <mongo.h>

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
   collection = mongoc_client_get_collection (client, "logistics", "testcrud");
   query = bson_new ();
   BSON_APPEND_UTF8 (query, "table_name", "eastchina");


    /*bson_t fields[1];  //这个里面表示你希望查询的结果返回哪些字段（貌似不应该叫字段）
    bson_init(fields);
    BSON_APPEND_BOOL(fields,"_id",0); //为1就是标识结果中需要返回这个字段
    BSON_APPEND_BOOL(fields,"table_id",0);
    BSON_APPEND_BOOL(fields,"content",1);*/
    //bson_finish(fields);

    //cursor = mongo_find(collection,"test.virus", query, fields, 9999, 0, 0);

   cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);
   bson_t* filter = BCON_NEW ("table_name", BCON_UTF8("eastchina"));
   bson_t* opts = BCON_NEW ("projection", "{",
                        "table_id", BCON_BOOL (false),
                        //"table_name", BCON_BOOL (true),
                        //"content", BCON_BOOL (true),
  //                      "_id", BCON_BOOL (false),
                        //"\"content.province_map.yiyiling.spot_num\"", BCON_BOOL (false),
                        "content.province_map.yiyiling.spot_num", BCON_BOOL (false),
                  "}");

   cursor = mongoc_collection_find_with_opts (collection, filter, opts, NULL);
   while (mongoc_cursor_next (cursor, &doc)) {
      str = bson_as_json (doc, NULL);
      printf ("%s\n", str);
      bson_free (str);
   }

   bson_destroy (query);
   bson_destroy (filter);
   bson_destroy (opts);
       //查询完了 别忘了释放
   //bson_destroy(fields);
   mongoc_cursor_destroy (cursor);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);
   mongoc_cleanup ();

   return 0;
}
