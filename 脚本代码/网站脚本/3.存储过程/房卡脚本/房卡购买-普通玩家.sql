----------------------------------------------------------------------------------------------------
-- ��Ȩ��2011
-- ʱ�䣺2012-02-23
-- ��;����������
----------------------------------------------------------------------------------------------------

USE RYTreasureDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].NET_PW_BuyRoomCardGeneral') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].NET_PW_BuyRoomCardGeneral
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_NULLS ON
GO

----------------------------------------------------------------------------------------------------

CREATE PROCEDURE NET_PW_BuyRoomCardGeneral
	@dwUserID	INT,						-- �û� I D
	@dwBuyNum	INT,						-- ��������
	@dwTypeID	INT,						-- ֧�����ͣ�0����Ϸ�� 1����Ϸ�ң�
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
DECLARE @UserGold BIGINT

-- ��Ʒ��Ϣ
DECLARE @Rate BIGINT
DECLARE @RoomCard INT
DECLARE @Currency DECIMAL(18,2)
DECLARE @Gold BIGINT

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

	--��Ϸ������
	IF @dwTypeID = 0
	BEGIN
		--��֤����
		SELECT @Rate = CAST(Cash AS BIGINT) FROM RYPlatformDBLink.RYPlatformDB.dbo.GameProperty WHERE ID = 501
		IF @Rate IS NULL OR @Rate <= 0
		BEGIN
			SET @strErrorDescribe=N'�ǳ���Ǹ,�����������δ���ã�����ϵ����Ա��'
			RETURN 4
		END

		-- ��ȡ֧����Ϸ��
		SET @Currency = @Rate * @dwBuyNum

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
			INSERT INTO UserRoomCard VALUES(@UserID,@dwBuyNum)
		END
		ELSE
		BEGIN
			UPDATE UserRoomCard SET RoomCard = RoomCard + @dwBuyNum WHERE UserID=@UserID
		END

		IF @@ROWCOUNT <= 0
		BEGIN
			SET @strErrorDescribe=N'�ǳ���Ǹ,����ʧ�ܣ�'
			ROLLBACK TRAN
			RETURN 6
		END

		COMMIT TRAN

		-- д�������¼
		INSERT INTO RYRecordDBLink.RYRecordDB.dbo.RecordRoomCard(SourceUserID,SBeforeCard,RoomCard,TypeID,Currency,SBeforeCurrency,ClientIP) VALUES(@UserID,@UserRoomCard,@dwBuyNum,0,@Currency,@UserCurrency,@strClientIP)
	END
	--��Ϸ�ҹ���
	ELSE
	BEGIN
		--��֤����
		SELECT @Rate = Gold FROM RYPlatformDBLink.RYPlatformDB.dbo.GameProperty WHERE ID = 501
		IF @Rate IS NULL OR @Rate <= 0
		BEGIN
			SET @strErrorDescribe=N'�ǳ���Ǹ,�����������δ���ã�����ϵ����Ա��'
			RETURN 4
		END

		-- ��ȡ֧����Ϸ��
		SET @Gold = @Rate * @dwBuyNum

		-- ��������
		BEGIN TRAN

		SELECT @UserGold=InsureScore FROM GameScoreInfo(ROWLOCK) WHERE UserID=@UserID
		IF @UserGold IS NULL OR @UserGold<@Gold
		BEGIN
			SET @strErrorDescribe=N'�ǳ���Ǹ,�������е���Ϸ�����㣡'
			ROLLBACK TRAN
			RETURN 5
		END

		UPDATE GameScoreInfo SET InsureScore = InsureScore - @Gold WHERE UserID=@UserID
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
			INSERT INTO UserRoomCard VALUES(@UserID,@dwBuyNum)
		END
		ELSE
		BEGIN
			UPDATE UserRoomCard SET RoomCard = RoomCard + @dwBuyNum WHERE UserID=@UserID
		END

		IF @@ROWCOUNT <= 0
		BEGIN
			SET @strErrorDescribe=N'�ǳ���Ǹ,����ʧ�ܣ�'
			ROLLBACK TRAN
			RETURN 6
		END

		COMMIT TRAN

		-- д�������¼
		INSERT INTO RYRecordDBLink.RYRecordDB.dbo.RecordRoomCard(SourceUserID,SBeforeCard,RoomCard,TypeID,Gold,SBeforeGold,ClientIP) VALUES(@UserID,@UserRoomCard,@dwBuyNum,4,@Gold,@UserGold,@strClientIP)
	END

	SET @strErrorDescribe=N'��������ɹ���' 
END

RETURN 0

GO