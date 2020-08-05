-- there is a table with test results from different tests
-- need to select only the rows with the last test results for each test
-- so one test result should be selected and it has to be the latest one

SELECT d.name, t.type, LEFT(platform, 3) AS platform, to_char(dt, 'YYYY-MM-DD HH:mm:ss') AS "Finished at", EXTRACT(epoch from now()-dt)/3600 AS "Hours passed"
FROM hspc_test_results AS r
    JOIN hspc_test_datasets AS d ON d.id = r.dataset_id
    JOIN hspc_test_types AS t ON t.id = d.type_id
WHERE r.id IN (
    SELECT id
    FROM (
        SELECT ROW_NUMBER() OVER (PARTITION BY dataset_id, LOWER(LEFT(platform, 3)) ORDER BY dt DESC) AS ROWNUMBER, id
        FROM hspc_test_results
        WHERE LOWER(LEFT(platform, 3)) != 'xfo'
    ) AS prt
    WHERE ROWNUMBER = 1
)
ORDER BY "Hours passed" DESC;
