SELECT *
FROM sys.dm_db_index_usage_stats
WHERE database_id = DB_ID( 'SomeBase_name') AND OBJECT_ID=OBJECT_ID('[SomeBase_name].[dbo].[tableNmae]')