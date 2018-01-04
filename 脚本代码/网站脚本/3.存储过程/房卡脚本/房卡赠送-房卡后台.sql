----------------------------------------------------------------------------------------------------
-- ��Ȩ��2011
-- ʱ�䣺2012-02-23
-- ��;�������һ�
----------------------------------------------------------------------------------------------------

USE RYTreasureDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].NET_PW_PresentRoomCard') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].NET_PW_PresentRoomCard
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_NULLS ON
GO

----------------------------------------------------------------------------------------------------

CREATE PROCEDURE NET_PW_PresentRoomCard
	@dwUserID	INT,						-- �û� I D
	@dwPresentCount INT,					-- ��������
	@dwGameID INT,							-- ����ID
	@strPassword NCHAR(32),					-- ��������
	@strClientIP NVARCHAR(15),				-- ������ַ
	@strDescript NVARCHAR(80),				-- ��ע˵��
	@strErrorDescribe NVARCHAR(127) OUTPUT	-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- �û���Ϣ
DECLARE @UserID INT
DECLARE @Nullity BIT
DECLARE @StunDown BIT
DECLARE @InsurePass NCHAR(32)
DECLARE @UserRoomCard INT

DECLARE @TUserID INT
DECLARE @TUserRoomCard INT


-- ִ���߼�
BEGIN
	-- ��ѯ�û�
	SELECT @UserID=UserID, @Nullity=Nullity, @StunDown=StunDown,@InsurePass=InsurePass FROM RYAccountsDBLink.RYAccountsDB.dbo.AccountsInfo WHERE UserID=@dwUserID
	SELECT @TUserID=UserID FROM RYAccountsDBLink.RYAccountsDB.dbo.AccountsInfo WHERE GameID=@dwGameID

	-- ��ѯ�û�
	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'�����ʺŲ����ڻ������������������֤���ٴγ��Ե�¼��'
		RETURN 1
	END	
	IF @TUserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'�������͵�Ŀ���˺Ų����ڣ����֤���ٴ����룡'
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

	-- ��������
	BEGIN TRAN

	SELECT @UserRoomCard=RoomCard FROM UserRoomCard(ROWLOCK) WHERE UserID=@UserID
	IF @UserRoomCard IS NULL OR @UserRoomCard < @dwPresentCount
	BEGIN
		SET @strErrorDescribe=N'�ǳ���Ǹ,���ķ������㣬���ȹ���'
		ROLLBACK TRAN
		RETURN 5
	END

	UPDATE UserRoomCard SET RoomCard = RoomCard - @dwPresentCount WHERE UserID=@UserID
	IF @@ROWCOUNT <= 0
	BEGIN
		SET @strErrorDescribe=N'�ǳ���Ǹ,����ʧ�ܣ�'
		ROLLBACK TRAN
		RETURN 6
	END

	SELECT @TUserRoomCard=RoomCard FROM UserRoomCard WHERE UserID = @TUserID
	IF @TUserRoomCard IS NULL
	BEGIN
		SET @TUserRoomCard = 0
		INSERT INTO UserRoomCard VALUES(@TUserID,@dwPresentCount)
	END
	ELSE
	BEGIN
		UPDATE UserRoomCard SET RoomCard = RoomCard + @dwPresentCount WHERE UserID = @TUserID
	END

	IF @@ROWCOUNT <= 0
	BEGIN
		SET @strErrorDescribe=N'�ǳ���Ǹ,����ʧ�ܣ�'
		ROLLBACK TRAN
		RETURN 6
	END

	COMMIT TRAN

	-- д�������¼
	INSERT INTO RYRecordDBLink.RYRecordDB.dbo.RecordRoomCard(SourceUserID,TargetUserID,SBeforeCard,TBeforeCard,RoomCard,TypeID,ClientIP,Remarks) VALUES(@UserID,@TUserID,@UserRoomCard,@TUserRoomCard,@dwPresentCount,1,@strClientIP,@strDescript)
	
	SET @strErrorDescribe=N'�������ͳɹ���' 
END

RETURN 0

GO