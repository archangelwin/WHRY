USE RYAccountsDB
GO
DECLARE @CountBegin  INT
DECLARE @CountEnd	 INT
DECLARE @Count		 INT
DECLARE @Name		 NVARCHAR(20)
DECLARE @FirstName	 NVARCHAR(20)
DECLARE @UserCount	 INT
DECLARE @MemberOrder INT

SELECT @CountBegin= IsNull(Max(UserID),1) From RYAccountsdb..AccountsInfo WHERE IsAndroid=1
/*------------------------�༭����ʼ----------------------------------------*/
SET @Count=	200					----�����ʺŵ�����
SET @FirstName='������'			----�����˺ŵ�ǰ׺
SET @UserCount = 50  			----��Ա����
SET @MemberOrder= 1				----��Ա���� Vip=1,Vip=2,Vip3=3,Vip4=4,Vip5=5
/*------------------------�༭�������----------------------------------------*/
SET @CountEnd=@CountBegin+@Count
WHILE(@CountBegin <= @CountEnd)

BEGIN
SET @Name = @FirstName+LTRIM(STR(@CountBegin))
INSERT INTO RYAccountsdb..AccountsInfo 
(Accounts,NickName,RegAccounts,LogonPass,SpreaderID,PassPortID,Compellation,Gender,FaceID,RegisterIP,RegisterMachine,LastLogonIP,IsAndroid)
VALUES  (@Name,@Name,@Name,N'',N'',N'',N'',RAND()*2,RAND()*100,'','','','1')
SET @CountBegin = @CountBegin + 1
END

UPDATE AccountsInfo SET AccountsInfo.GameID=GameIdentifier.GameID 
FROM AccountsInfo,GameIdentifier 
WHERE AccountsInfo.UserID NOT IN (SELECT UserID FROM RYAccountsDB..AndroidLockInfo) AND IsAndroid=1 AND AccountsInfo.UserID=GameIdentifier.UserID

UPDATE TOP(@UserCount)A Set MemberOrder=@MemberOrder
From (Select TOP (@UserCount) * From AccountsInfo WHERE Isandroid=1 and MemberOrder=0 Order by Newid())A
INSERT INTO AndroidLockInfo(UserID,MemberOrder) 
SELECT UserID,MemberOrder FROM AccountsInfo WHERE IsAndroid=1 AND UserID NOT IN(SELECT UserID FROM AndroidLockInfo)