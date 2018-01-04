----------------------------------------------------------------------
-- ��Ȩ��2011
-- ʱ�䣺2011-09-1
-- ��;�����߳�ֵ
----------------------------------------------------------------------

USE [RYTreasureDB]
GO

-- ���߳�ֵ
IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].NET_PW_FilledOnLineFK') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].NET_PW_FilledOnLineFK
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

---------------------------------------------------------------------------------------
-- ���߳�ֵ
CREATE PROCEDURE NET_PW_FilledOnLineFK
	@strOrdersID		NVARCHAR(50),			--	�������
	@PayAmount			DECIMAL(18,2),			--  ֧�����
	@strIPAddress		NVARCHAR(31),			--	�û��ʺ�	
	@strErrorDescribe	NVARCHAR(127) OUTPUT	--	�����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ������Ϣ
DECLARE @OperUserID INT
DECLARE @ShareID INT
DECLARE @UserID INT
DECLARE @GameID INT
DECLARE @Accounts NVARCHAR(31)
DECLARE @OrderAmount DECIMAL(18,2)
DECLARE @IPAddress NVARCHAR(15)
DECLARE @OrderID NVARCHAR(50)
DECLARE @RoomCard INT
DECLARE @BeforeRoomCard INT

-- �û���Ϣ
DECLARE @Score BIGINT

-- ������Ϣ
DECLARE @Rate INT

-- ִ���߼�
BEGIN
	-- ������ѯ
	SELECT @OperUserID=OperUserID,@ShareID=ShareID,@UserID=UserID,@GameID=GameID,@Accounts=Accounts,
		@OrderID=OrderID,@OrderAmount=OrderAmount,@RoomCard=RoomCard  
	FROM OnLineOrder WHERE OrderID=@strOrdersID

	-- ��������
	IF @OrderID IS NULL 
	BEGIN
		SET @strErrorDescribe=N'��Ǹ����ֵ���������ڡ�'
		RETURN 1
	END

	-- �����ظ�
	IF EXISTS(SELECT OrderID FROM ShareDetailInfo(NOLOCK) WHERE OrderID=@strOrdersID) 
	BEGIN
		SET @strErrorDescribe=N'��Ǹ����ֵ�����ظ���'
		RETURN 2
	END

	-- ��֤���
	IF @OrderAmount != @PayAmount
	BEGIN
		SET @strErrorDescribe=N'��Ǹ��֧��������'
		RETURN 2
	END

	-- ������ѯ
	SELECT @BeforeRoomCard=RoomCard FROM UserRoomCard WHERE UserID=@UserID
	IF @BeforeRoomCard IS NULL
	BEGIN
		SET @BeforeRoomCard=0
		INSERT INTO UserRoomCard VALUES(@UserID,@RoomCard)
	END
	ELSE
	BEGIN
		UPDATE UserRoomCard SET RoomCard = RoomCard + @RoomCard WHERE UserID=@UserID
	END

	-- ������¼
	INSERT INTO ShareDetailInfo(
		OperUserID,ShareID,UserID,GameID,Accounts,OrderID,OrderAmount,DiscountScale,PayAmount,
		RoomCard,BeforeRoomCard,IPAddress)
	VALUES(
		@OperUserID,@ShareID,@UserID,@GameID,@Accounts,@OrderID,@OrderAmount,1,@PayAmount,
		@RoomCard,@BeforeRoomCard,@strIPAddress)

	-- ���¶���״̬
	UPDATE OnLineOrder SET OrderStatus=2,PayAmount=@PayAmount 
	WHERE OrderID=@OrderID

	--------------------------------------------------------------------------------
	-- �ƹ�ϵͳ&����ϵͳ
	--DECLARE @SpreaderID INT	
	--SELECT @SpreaderID=SpreaderID FROM RYAccountsDBLink.RYAccountsDB.dbo.AccountsInfo
	--WHERE UserID = @UserID
	--IF @SpreaderID<>0
	--BEGIN
	--	-- �������ҵĻ���
	--	DECLARE @GoldRate INT
	--	SELECT @GoldRate=StatusValue FROM RYAccountsDBLink.RYAccountsDB.dbo.SystemStatusInfo WHERE StatusName='RateGold'
	--	IF @GoldRate=0 OR @GoldRate IS NULL
	--		SET @GoldRate=1

	--	-- ����ϵͳ
	--	DECLARE @AgentUserID INT
	--	DECLARE @AgentType INT
	--	DECLARE @AgentScale DECIMAL(18,3)
	--	DECLARE @PayScore BIGINT
	--	DECLARE @AgentScore BIGINT
	--	DECLARE @AgentDateID INT	
	--	SELECT @AgentUserID=UserID,@AgentType=AgentType,@AgentScale=AgentScale FROM RYAccountsDBLink.RYAccountsDB.dbo.AccountsAgent WHERE UserID=@SpreaderID AND Nullity=0
	--	IF @AgentUserID IS NOT NULL
	--	BEGIN
	--		IF @AgentType=1 -- ��ֵ�ֳ�
	--		BEGIN
	--			-- ��ֵ��Ҽ���
	--			SET @PayScore=@Currency*@GoldRate
	--			SET @AgentScore=@PayScore*@AgentScale
	--			SET @AgentDateID=CAST(CAST(GETDATE() AS FLOAT) AS INT)	
	--			-- �����ֳɼ�¼
	--			INSERT INTO RecordAgentInfo(DateID,UserID,AgentScale,TypeID,PayScore,Score,ChildrenID,CollectIP) VALUES(@AgentDateID,@AgentUserID,@AgentScale,1,@PayScore,@AgentScore,@UserID,@strIPAddress)
	--			-- ������ͳ��
	--			UPDATE StreamAgentPayInfo SET PayAmount=PayAmount+@PayAmount,Currency=Currency+@Currency,PayScore=PayScore+@PayScore,LastCollectDate=GETDATE() WHERE DateID=@AgentDateID AND UserID=@AgentUserID
	--			IF @@ROWCOUNT=0
	--			BEGIN
	--				INSERT INTO StreamAgentPayInfo(DateID,UserID,PayAmount,Currency,PayScore) VALUES(@AgentDateID,@AgentUserID,@PayAmount,@Currency,@PayScore)
	--			END
	--		END
	--	END
	--	ELSE -- �ƹ�ϵͳ
	--	BEGIN
	--		DECLARE @SpreadRate DECIMAL(18,2)
	--		DECLARE @GrantScore BIGINT
	--		DECLARE @Note NVARCHAR(512)
	--		-- �ƹ�ֳ�
	--		SELECT @SpreadRate=FillGrantRate FROM GlobalSpreadInfo
	--		IF @SpreadRate IS NULL
	--		BEGIN
	--			SET @SpreadRate=0.1
	--		END
			
	--		SET @GrantScore = @Currency*@GoldRate*@SpreadRate
	--		SET @Note = N'��ֵ'+LTRIM(STR(@PayAmount))+'Ԫ'
	--		INSERT INTO RecordSpreadInfo(UserID,Score,TypeID,ChildrenID,CollectNote)
	--		VALUES(@SpreaderID,@GrantScore,3,@UserID,@Note)		
	--	END	
	--END

	-- ��¼��־
	DECLARE @DateID INT
	SET @DateID=CAST(CAST(GETDATE() AS FLOAT) AS INT)

	UPDATE StreamShareInfo
	SET ShareTotals=ShareTotals+1
	WHERE DateID=@DateID AND ShareID=@ShareID

	IF @@ROWCOUNT=0
	BEGIN
		INSERT StreamShareInfo(DateID,ShareID,ShareTotals)
		VALUES (@DateID,@ShareID,1)
	END	 
	
END 
RETURN 0
GO



