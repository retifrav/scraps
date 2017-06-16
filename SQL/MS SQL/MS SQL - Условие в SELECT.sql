SELECT field1,
       field2,
       CASE type_client
       WHEN 0
            THEN brFL.name
            ELSE brUL.name
       END AS branch,
       field3
FROM request AS r
     LEFT JOIN branchesFL AS brFL
          ON r.branchID = brFL.IdBranch
     LEFT JOIN branchesUL AS brUL
          ON r.branchID = brUL.IdBranch