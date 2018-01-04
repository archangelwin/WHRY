----------------------------------------------------------------------
-- ��Ȩ��2011
-- ʱ�䣺2011-09-1
-- ��;�����߷�������
----------------------------------------------------------------------

USE [RYTreasureDB]
GO

-- ���붩��
IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].NET_PW_ApplyOnLineOrderFK') AND OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].NET_PW_ApplyOnLineOrderFK
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

----------------------------------------------------------------------------------
-- ���붩��
CREATE PROCEDURE NET_PW_ApplyOnLineOrderFK
	@strOrderID			NVARCHAR(32),				-- ������ʶ
	@dwOperUserID		INT,						-- �����û�

	@dwShareID			INT,						-- ��������
	@dwConfigID			INT,						-- ��ֵ����
	@dwGameID			INT,						-- ��ֵID
	
	@strIPAddress		NVARCHAR(15),				-- ֧����ַ
	@strErrorDescribe	NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- �ʺ�����
DECLARE @Accounts NVARCHAR(31)
DECLARE @GameID INT
DECLARE @UserID INT
DECLARE @Nullity TINYINT
DECLARE @StunDown TINYINT

-- ������Ϣ
DECLARE @OrderID NVARCHAR(32)
DECLARE @RoomCard INT
DECLARE @Amount INT

-- ִ���߼�
BEGIN
	-- ��֤�û�
	SELECT @UserID=UserID,@GameID=GameID,@Accounts=Accounts,@Nullity=Nullity,@StunDown=StunDown
	FROM RYAccountsDBLink.RYAccountsDB.dbo.AccountsInfo
	WHERE GameID=@dwGameID

	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'��Ǹ����Ҫ��ֵ���û��˺Ų����ڡ�'
		RETURN 1
	END

	IF @Nullity=1
	BEGIN
		SET @strErrorDescribe=N'��Ǹ����Ҫ��ֵ���û��˺���ʱ���ڶ���״̬������ϵ�ͻ����������˽���ϸ�����'
		RETURN 2
	END

	IF @StunDown<>0
	BEGIN
		SET @strErrorDescribe=N'��Ǹ����Ҫ��ֵ���û��˺�ʹ���˰�ȫ�رչ��ܣ��������¿�ͨ����ܼ���ʹ�á�'
		RETURN 3
	END

	-- ������ѯ
	SELECT @OrderID=OrderID FROM OnLineOrder WHERE OrderID=@strOrderID
	IF @OrderID IS NOT NULL
	BEGIN
		SET @strErrorDescribe=N'��Ǹ���ö����Ѵ���,�����³�ֵ��'
		RETURN 4
	END

	-- ��������
	--IF EXISTS (SELECT UserID FROM GameScoreLocker(NOLOCK) WHERE UserID=@UserID)
	--BEGIN
	--	SET @strErrorDescribe='��Ǹ�����Ѿ��ڽ����Ϸ�����ˣ����ܽ��г�ֵ�����������˳������Ϸ���䣡'	
	--	RETURN 5
	--END
	
	-- ��ֵ����
	SELECT @RoomCard=RoomCard,@Amount=Amount FROM RoomCardConfig WHERE ConfigID = @dwConfigID AND Nullity = 0
	IF @Amount IS NULL OR @Amount <=0
	BEGIN
		SET @strErrorDescribe=N'��Ǹ����ֵ������ò����ڡ�'
		RETURN 4
	END

	-- ��������
	INSERT INTO OnLineOrder(
		OperUserID,ShareID,UserID,GameID,Accounts,OrderID,OrderAmount,PayAmount,Rate,RoomCard,IPAddress)
	VALUES(
		@dwOperUserID,@dwShareID,@UserID,@GameID,@Accounts,@strOrderID,@Amount,0,0,@RoomCard,@strIPAddress)
	
	SELECT @dwOperUserID AS OperUserID,@dwShareID AS ShareID,@UserID AS UserID,@GameID AS GameID,@Accounts AS Accounts,
		   @strOrderID AS OrderID, @Amount AS OrderAmount,0 AS PayAmount,0 AS Rate,0 AS Currency,@strIPAddress AS IPAddress

END
RETURN 0
GO



