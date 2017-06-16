SELECT name, type
FROM sys.procedures
--FROM sys.tables
--FROM sys.views
WHERE name LIKE '%ololo%'
--WHERE Object_definition(object_id) LIKE '%SELECT ololo FROM%'