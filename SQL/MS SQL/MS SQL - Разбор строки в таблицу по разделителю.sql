DECLARE @stringToSplit nvarchar(300) = '1,2,3,4,5,6'
DECLARE @returnList TABLE ([Name] [nvarchar] (100))

DECLARE @name NVARCHAR(50)
DECLARE @pos INT

WHILE CHARINDEX(',', @stringToSplit) > 0
BEGIN
    SELECT @pos  = CHARINDEX(',', @stringToSplit)  
    SELECT @name = SUBSTRING(@stringToSplit, 1, @pos-1)
    
    INSERT INTO @returnList 
    SELECT @name
    
    SELECT @stringToSplit = SUBSTRING(@stringToSplit, @pos+1, LEN(@stringToSplit)-@pos)
END

INSERT INTO @returnList
SELECT @stringToSplit

SELECT * FROM @returnList