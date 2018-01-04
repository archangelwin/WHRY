USE RYAccountsDB
GO

CREATE TABLE #T(UserName NVARCHAR(50)) 

BULK INSERT #T FROM 'D:\NAME.TXT' WITH(ROWTERMINATOR='\n')
SELECT ID=IDENTITY(INT), #T.UserName INTO #REGTEMP FROM #T 

DECLARE @I INT,@COUNT INT ,@BEGINID INT 
SET @I=1 SET @BEGINID=1 

SELECT @COUNT=COUNT(*) FROM #T /*@count=6031*/
SET @COUNT=@COUNT+@I/*6310+1*/WHILE(@I<=@COUNT)/*1<=6032*/ 

BEGIN 
	UPDATE  AccountsInfo
		SET Accounts=T.UserName,
			NickName=T.UserName,
			RegAccounts=T.USERNAME FROM AccountsInfo,#REGTEMP AS T 
			WHERE IsAndroid=1 AND T.ID=@I /*�����������ǻ����ˡ���ʱ��#REGTEMP.ID���ڱ���@I*/
			AND	UserID IN	 (SELECT TOP(@I)UserID FROM AccountsInfo WHERE UserID>=@BEGINID)/*UserID������*/
			AND	UserID NOT IN(SELECT TOP(@I-1)ISNULL(UserID,0)FROM AccountsInfo WHERE UserID>=@BEGINID)  
SET @I=@I+1
END 
---------------------------------------
DROP TABLE #T 
DROP TABLE #REGTEMP