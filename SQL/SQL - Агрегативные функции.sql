-- ERROR: Column 'logs.metrika.dt' is invalid in the select list because it is not contained in either an aggregate function or the GROUP BY clause.

-- Put in other words, this error is telling you that SQL Server does not know which B to select from the group. 

-- Either you want to select one specific value (e.g. the MIN, SUM, or AVG) in which case you would use the appropriate aggregate function, or you want to select every value as a new row (i.e. including B in the GROUP BY field list).

-- Consider the following data:
ID  A   B
1   1  13
1   1  79
1   2  13
1   2  13
1   2  42

-- The query
SELECT A, COUNT(B) AS T1 
FROM T2 
GROUP BY A
-- would return:
A  T1
1  2
2  3

-- which is all well and good. 

-- However consider the following (illegal) query, which would produce this error:
SELECT A, COUNT(B) AS T1, B 
FROM T2 
GROUP BY A
-- And its returned data set illustrating the problem:
A  T1  B
1  2   13? 79? Both 13 and 79 as separate rows? (13+79=92)? ...?
2  3   13? 42? ...?

-- However, the following two queries make this clear, and will not cause the error:
-- 1. Using an aggregate
SELECT A, COUNT(B) AS T1, SUM(B) AS B
FROM T2
GROUP BY A
-- would return:
A  T1  B
1  2   92
2  3   68

-- 2. Adding the column to the GROUP BY list
SELECT A, COUNT(B) AS T1, B
FROM T2
GROUP BY A, B
-- would return:
A  T1  B
1  1   13
1  1   79
2  2   13
2  1   42