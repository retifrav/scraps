Допустим, такая структура поля:

<request id="">
  <inquiry>435474</inquiry>
  <idbranch>546</idbranch>
  <idcredit>7</idcredit>
  <surname>Иванов</surname>
  <name>Виталий</name>
  <patron>Иванович</patron>
  <dob>21.11.1991</dob>
</request>

-- поиск записей по значению тега
SELECT *
FROM [Checks].[Data].[Tasks]
WHERE SearchRequest.exist('/request/surname') = 1 -- если нет такого тега, то провал
  AND SearchRequest.query('/request/idcredit').value('.[1]', 'nvarchar(30)') = '7'

-- изменение значения тега
UPDATE [Checks].[Data].[Tasks]
SET SearchRequest.modify(
   'replace value of (/request/idcredit/text())[1] with "карточка"'
   )
WHERE ID = 234