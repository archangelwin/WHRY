----------------------------------------------------------------------------------------------------
-- ��Ȩ��2011
-- ʱ�䣺2012-02-23
-- ��;�������һ�
----------------------------------------------------------------------------------------------------

USE RYTreasureDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].NET_PW_ExchRoomCard') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].NET_PW_ExchRoomCard
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_NULLS ON
GO

----------------------------------------------------------------------------------------------------

CREATE PROCEDURE NET_PW_ExchRoomCard
	@dwUserID	INT,						-- �û� I D
	@dwExchCount INT,						-- �һ�����
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
DECLARE @UserGold BIGINT
DECLARE @UserRoomCard INT
DECLARE @RegisterIP NVARCHAR(15)

-- �һ���Ϣ
DECLARE @Rate INT
DECLARE @Gold INT

-- ִ���߼�
BEGIN
	-- ��ѯ�û�
	SELECT @UserID=UserID, @Nullity=Nullity, @StunDown=StunDown,@InsurePass=InsurePass,@RegisterIP=RegisterIP FROM RYAccountsDBLink.RYAccountsDB.dbo.AccountsInfo WHERE UserID=@dwUserID

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

	-- ��ȡ�һ�����
	SELECT @Rate=StatusValue FROM RYAccountsDBLink.RYAccountsDB.dbo.SystemStatusInfo WHERE StatusName =N'AgentRoomCardExchRate'
	IF @Rate IS NULL
	BEGIN
		SET @strErrorDescribe=N'�ǳ���Ǹ�������һ�������δ���ã�'
		RETURN 4
	END

	-- ����һ���Ϸ��
	SET @Gold = @Rate * @dwExchCount

	-- ��������
	BEGIN TRAN

	SELECT @UserRoomCard=RoomCard FROM UserRoomCard(ROWLOCK) WHERE UserID=@UserID
	IF @UserRoomCard IS NULL OR @UserRoomCard < @dwExchCount
	BEGIN
		SET @strErrorDescribe=N'�ǳ���Ǹ,���ķ������㣬���ȹ���'
		ROLLBACK TRAN
		RETURN 5
	END

	UPDATE UserRoomCard SET RoomCard = RoomCard - @dwExchCount WHERE UserID=@UserID
	IF @@ROWCOUNT <= 0
	BEGIN
		SET @strErrorDescribe=N'�ǳ���Ǹ,�һ�ʧ�ܣ�'
		ROLLBACK TRAN
		RETURN 6
	END

	SELECT @UserGold=Score FROM GameScoreInfo WHERE UserID=@UserID
	IF @UserGold IS NULL
	BEGIN
		SET @UserGold = 0
		INSERT INTO GameScoreInfo (UserID, Score, RegisterIP, LastLogonIP) VALUES (@UserID, @Gold, @RegisterIP, @RegisterIP) 
	END
	ELSE
	BEGIN
		UPDATE GameScoreInfo SET Score = Score + @Gold WHERE UserID=@UserID
	END

	IF @@ROWCOUNT <= 0
	BEGIN
		SET @strErrorDescribe=N'�ǳ���Ǹ,�һ�ʧ�ܣ�'
		ROLLBACK TRAN
		RETURN 6
	END

	COMMIT TRAN

	-- д�������¼
	INSERT INTO RYRecordDBLink.RYRecordDB.dbo.RecordRoomCard(SourceUserID,SBeforeCard,RoomCard,TypeID,Gold,SBeforeGold,ClientIP) VALUES(@UserID,@UserRoomCard,@dwExchCount,2,@Gold,@UserGold,@strClientIP)
	
	SET @strErrorDescribe=N'�����һ��ɹ���' 
END

RETURN 0

GO