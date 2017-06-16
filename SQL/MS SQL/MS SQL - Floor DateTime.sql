In SQL Server, how do I "floor" a DATETIME to the second/minute/hour/day/year?

-- Let's say that I have a date of 2008-09-17 12:56:53.430, then the output of flooring should be:
    
    Year: 2008-01-01 00:00:00.000
    Month: 2008-09-01 00:00:00.000
    Day: 2008-09-17 00:00:00.000
    Тут ещё неделя есть
    Hour: 2008-09-17 12:00:00.000
    Minute: 2008-09-17 12:56:00.000
    Second: 2008-09-17 12:56:53.000

--The key is to use DATEADD and DATEDIFF along with the appropriate SQL timespan enumeration.

    declare @datetime datetime;
    set @datetime = getdate();
    SELECT @datetime;
    SELECT dateadd(year,datediff(year,0,@datetime),0);
    SELECT dateadd(month,datediff(month,0,@datetime),0);
    SELECT dateadd(week,datediff(week,0,GETDATE()),0);
    SELECT dateadd(day,datediff(day,0,@datetime),0);
    SELECT dateadd(hour,datediff(hour,0,@datetime),0);
    SELECT dateadd(minute,datediff(minute,0,@datetime),0);
    SELECT dateadd(second,datediff(second,'2000-01-01',@datetime),'2000-01-01');

-- Note that when you are flooring by the second, you will often get an arithmetic overflow if you use 0. So pick a known value that is guaranteed to be lower than the datetime you are attempting to floor.