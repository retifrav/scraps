declare @t table (name nvarchar(250), mail nvarchar(250), idparent int)  

insert @t
SELECT e.name, e.mail, e.codeParent
FROM somedb.dbo.Employee AS e
     
     JOIN somedb.dbo.City AS c
          ON c.ID = e.CodeParent
     JOIN somedb.dbo.Filials AS f
          ON c.IdComp = f.Id_ROO
WHERE f.ID = 'asd' AND e.pfof LIKE 'Руководитель группы%' AND e.Specification = 1

select *
from @t
union all
select distinct e.name, e.mail, 0
from @t as t
left join somedb.dbo.Employee as e
on e.code = t.idparent