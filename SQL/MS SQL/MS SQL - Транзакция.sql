CREATE PROCEDURE [dbo].[updateRequestStatus]
    @id_request                 bigint,
    @id_status                  tinyint,
    @statusDetails              nvarchar(300)   = NULL
AS
BEGIN

    set nocount on;

    declare @trancount int;
    set @trancount = @@trancount;
    
    BEGIN TRY

        if @trancount = 0
            begin transaction
        else
            save transaction createNewTransportMessage;

        DECLARE @currentStatus tinyint
        SELECT @currentStatus = [id_requestStatus]
        FROM [dbo].[requests]
        WHERE [id_request] = @id_request

        -- не давать даунгрейдить статус
        IF (@id_status < @currentStatus AND @id_status != 0)
        BEGIN
            ROLLBACK TRANSACTION
            RETURN
        END

        -- статус и дата изменения
        UPDATE [dbo].[requests]
        SET [id_requestStatus] = @id_status, dt_lastUpdate = GETDATE()
        WHERE [id_request] = @id_request

        -- комментарии к статусу
        IF (@statusDetails IS NOT NULL)
        BEGIN
            UPDATE [dbo].[requests]
            SET [statusDetails] = @statusDetails
            WHERE [id_request] = @id_request
        END

        if @trancount = 0 commit;

    END TRY

    BEGIN CATCH
       
       DECLARE @error int, @message varchar(4000), @xstate int;

        SELECT @error = ERROR_NUMBER(),
               @message = ERROR_MESSAGE(),
               @xstate = XACT_STATE()

        --SELECT 
        --    ERROR_NUMBER() AS ErrorNumber
        --    ,ERROR_SEVERITY() AS ErrorSeverity
        --    ,ERROR_STATE() AS ErrorState
        --    ,ERROR_PROCEDURE() AS ErrorProcedure
        --    ,ERROR_LINE() AS ErrorLine
        --    ,ERROR_MESSAGE() AS ErrorMessage;
        
        if @xstate = -1 
            rollback;
        if @xstate = 1 and @trancount = 0
            rollback;
        if @xstate = 1 and @trancount > 0
            rollback transaction createNewTransportMessage;
        
        RAISERROR('createNewTransportMessage: %d: %s', 16, 1, @error, @message);

    END CATCH;

END