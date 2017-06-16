CREATE PROCEDURE [dbo].[searchConfirmations] 
	@Idsmth int = NULL,
	@IdInquiryOut varchar(16) = NULL,
	@IdBranch int = NULL,
	@OrgName nvarchar(MAX) = NULL,
	@DateFrom date = '2013-09-01',
	@DateTo date = NULL,
	@IsActive tinyint = NULL,
	@ManagerMail nvarchar(MAX) = null
AS
BEGIN
	SET NOCOUNT ON;

	DECLARE @DateToCurrent date = DATEADD(DAY,1,GETDATE())
	if (@DateTo IS NULL) set @DateTo = @DateToCurrent
 
	SELECT ccfc.IdCancel, ccfc.IsActive, ccfc.CreateDate
	FROM somedb.Data.ConformsCancelsmth AS ccfc
		 JOIN somedb.Data.smths AS fc
			  ON ccfc.Idsmth = fc.Idsmth
		 JOIN somedb.Data.LegalPersons AS lp
		      ON fc.IdLP = lp.IdLP
		 JOIN somedb.Data.Participants AS p
		      ON fc.IdLP = p.IdLP
		 JOIN somedb.Data.Inquiries AS i
		      ON p.IdInquiry = i.IdInquiry
	WHERE 1 = 1 -- ну и вот, короче, суть, что первое условие надо типа такого,
		  -- а потом писать конструкции AND (@ПЕРЕМЕННАЯ IS NULL OR ПОЛЕ = @ПЕРЕМЕННАЯ)
          -- типа если @ПЕРЕМЕННАЯ не равна NULL, то искать по этому условию,
          -- иначе игнорировать это условие
          AND (@Idsmth IS NULL OR ccfc.Idsmth = @Idsmth)
	      AND (@IdInquiryOut IS NULL OR i.IdInquiryOut = @IdInquiryOut)
	      AND (@IdBranch IS NULL OR i.IdBranch = @IdBranch)
	      AND (@OrgName IS NULL OR lp.OrgName = @OrgName)
	      AND (@IsActive IS NULL OR ccfc.IsActive = @IsActive)
	      AND ((ccfc.CreateDate >= @DateFrom AND ccfc.CreateDate < @DateTo) OR ccfc.CreateDate IS NULL)
	      AND ccfc.ManagerMail IS NOT NULL
	      AND (@ManagerMail IS NULL OR ccfc.ManagerMail = @ManagerMail)
	      AND ccfc.isNP = 0

    ORDER BY CreateDate ASC
END

GO