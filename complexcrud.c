#include <bcon.h>
#include <bson.h>
#include <mongoc.h>
#include <stdio.h>
//单个文档内部增加字段
int
main (int argc, char *argv[])
{
   mongoc_collection_t *collection;
   mongoc_client_t *client;
   bson_error_t error;
   bson_t *update = NULL;
   bson_t *query = NULL;

   mongoc_init ();

   client = mongoc_client_new("mongodb://yourip:port");
   collection = mongoc_client_get_collection (client, "logistics", "testcrud");

   query = BCON_NEW ("table_name", "eastchina");
   //update = BCON_NEW ("$set", "{", "content.province_map.yiyiling.snd", BCON_UTF8 ("newfield"), "}");
   //update = BCON_NEW ("$set", "{", "content.province_map.yiyiling.spot_num", BCON_INT32 (88), "}");
   //update = BCON_NEW ("$inc", "{", "content.province_map.yiyiling.spot_num", BCON_INT32 (1), "}");
   //update = BCON_NEW ("$unset", "{", "content.province_map.yiyiling.modified", "", "}");
   //update = BCON_NEW ("$set", "{", "content.province_map.yiyiling.spot_ids", "{", "111000789", BCON_UTF8 ("createtime"),"}", "}");
   update = BCON_NEW ("$set", "{", "content.province_map.yiyiling.spot_ids.111000791", BCON_UTF8 ("trdtime"), "}");

   if (!mongoc_collection_update (
          collection, MONGOC_UPDATE_NONE, query, update, NULL, &error)) {
      fprintf (stderr, "%s\n", error.message);
      goto fail;
   }

fail:
   if (query)
      bson_destroy (query);
   if (update)
      bson_destroy (update);

   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);
   mongoc_cleanup ();

   return 0;
}
