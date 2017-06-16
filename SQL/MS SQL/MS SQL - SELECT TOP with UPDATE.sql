CREATE PROCEDURE [dbo].[getRequestsFromQueue]
    @howmch INT
AS
BEGIN
    SET NOCOUNT ON;
    
    -- это важно, чтобы с разных серверов не забрало одновременно одни и те же реквесты
    SET TRANSACTION ISOLATION LEVEL REPEATABLE READ

    BEGIN TRAN

        DECLARE @rez TABLE(id BIGINT)
        ;WITH CTE AS 
        ( 
            SELECT TOP (@howmch) id_request, taken
            FROM dbo.requests
            WHERE [taken] = 0
            ORDER BY priority ASC, dateIncoming
        )
        UPDATE CTE SET [taken] = 1
        OUTPUT INSERTED.id_request INTO @rez
        
        SELECT id FROM @rez

    COMMIT TRAN

    -- вернули как было
    SET TRANSACTION ISOLATION LEVEL READ COMMITTED

END
GO