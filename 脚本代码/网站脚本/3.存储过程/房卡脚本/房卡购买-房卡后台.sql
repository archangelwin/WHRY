----------------------------------------------------------------------------------------------------
-- ��Ȩ��2011
-- ʱ�䣺2012-02-23
-- ��;����������
----------------------------------------------------------------------------------------------------

USE RYTreasureDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].NET_PW_BuyRoomCard') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].NET_PW_BuyRoomCard
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_NULLS ON
GO

----------------------------------------------------------------------------------------------------

CREATE PROCEDURE NET_PW_BuyRoomCard
	@dwUserID	INT,						-- �û� I D
	@dwConfigID	INT,						-- �����Ʒ
	@strPassword NCHAR(32),					-- ��������
	@strClientIP NVARCHAR(15),				-- ������ַ
	@strErrorDescribe NVARCHAR(127) OUTPUT	-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- �û���Ϣ
DECLARE @UserID INT
DECLARE @Nullity BIT
DECLARE @StunDown BIT
DECLARE @InsurePass NCHAR(32)
DECLARE @UserCurrency DECIMAL(18,2)
DECLARE @UserRoomCard INT

-- ��Ʒ��Ϣ
DECLARE @RoomCard INT
DECLARE @Currency DECIMAL(18,2)

-- ִ���߼�
BEGIN
	-- ��ѯ�û�
	SELECT @UserID=UserID, @Nullity=Nullity, @StunDown=StunDown,@InsurePass=InsurePass FROM RYAccountsDBLink.RYAccountsDB.dbo.AccountsInfo WHERE UserID=@dwUserID

	-- ��ѯ�û�
	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'�����ʺŲ����ڻ������������������֤���ٴγ��Ե�¼��'
		RETURN 1
	END	
	
	-- �ʺŽ�ֹ
	IF @Nullity<>0
	BEGIN
		SET @strErrorDescribe=N'�����ʺ���ʱ���ڶ���״̬������ϵ�ͻ����������˽���ϸ�����'
		RETURN 2
	END	

	-- �ʺŹر�
	IF @StunDown<>0
	BEGIN
		SET @strErrorDescribe=N'�����ʺ�ʹ���˰�ȫ�رչ��ܣ��������¿�ͨ����ܼ���ʹ�ã�'
		RETURN 3
	END		

	-- ��֤��������
	IF @InsurePass!=@strPassword
	BEGIN
		SET @strErrorDescribe=N'�ǳ���Ǹ���������������������'
		RETURN 3
	END

	-- ��֤��Ʒ
	SELECT @RoomCard=RoomCard,@Currency=Currency FROM RoomCardConfig WHERE ConfigID = @dwConfigID
	IF @RoomCard IS NULL
	BEGIN
		SET @strErrorDescribe=N'�ǳ���Ǹ,������ķ�����Ʒδ���ã�'
		RETURN 4
	END

	-- ��������
	BEGIN TRAN

	SELECT @UserCurrency=Currency FROM UserCurrencyInfo(ROWLOCK) WHERE UserID=@UserID
	IF @UserCurrency IS NULL OR @UserCurrency<@Currency
	BEGIN
		SET @strErrorDescribe=N'�ǳ���Ǹ,������Ϸ�����㣬���ȳ�ֵ��'
		ROLLBACK TRAN
		RETURN 5
	END

	UPDATE UserCurrencyInfo SET Currency = Currency - @Currency WHERE UserID=@UserID
	IF @@ROWCOUNT <= 0
	BEGIN
		SET @strErrorDescribe=N'�ǳ���Ǹ,����ʧ�ܣ�'
		ROLLBACK TRAN
		RETURN 6
	END

	SELECT @UserRoomCard=RoomCard FROM UserRoomCard WHERE UserID=@UserID
	IF @UserRoomCard IS NULL
	BEGIN
		SET @UserRoomCard = 0
		INSERT INTO UserRoomCard VALUES(@UserID,@RoomCard)
	END
	ELSE
	BEGIN
		UPDATE UserRoomCard SET RoomCard = RoomCard + @RoomCard WHERE UserID=@UserID
	END

	IF @@ROWCOUNT <= 0
	BEGIN
		SET @strErrorDescribe=N'�ǳ���Ǹ,����ʧ�ܣ�'
		ROLLBACK TRAN
		RETURN 6
	END

	COMMIT TRAN

	-- д�������¼
	INSERT INTO RYRecordDBLink.RYRecordDB.dbo.RecordRoomCard(SourceUserID,SBeforeCard,RoomCard,TypeID,Currency,SBeforeCurrency,ClientIP) VALUES(@UserID,@UserRoomCard,@RoomCard,0,@Currency,@UserCurrency,@strClientIP)
	
	SET @strErrorDescribe=N'��������ɹ���' 
END

RETURN 0

GO