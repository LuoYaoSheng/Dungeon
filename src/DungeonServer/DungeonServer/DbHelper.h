#pragma once

struct sqlite3 ;
struct sqlite3_stmt ;

enum RESULT_TYPE
{
	RESULT_ERROR,
	RESULT_ROW,
	RESULT_DONE
} ;

class DbHelper
{
public:
	DbHelper(const char* sql) ;
	~DbHelper() ;

	static bool Initialize(const char* connInfoStr) ;
	static void Finalize() ;

	static bool Execute(const char* format, ...) ;
	
	/// �Ʒ��� DbHelper ��ü�� �����ϰ� ����ؾ� ��

	bool BindParamInt(int param) ;
	bool BindParamDouble(double param) ;
	bool BindParamText(const char* text, int count) ;

	RESULT_TYPE FetchRow() ;

	/// FetchRow�� �����Ͱ� �ִٸ� �ݵ�� ���ʴ�� ȣ���ؾ���
	int GetResultParamInt() ;
	double GetResultParamDouble() ;
	const unsigned char* GetResultParamText() ;

	int GetResultColCount() { return mResultColCount; }
	RESULT_TYPE GetResult() {	return mResult; }

private:

	static sqlite3* mSqlite ;

	/// ���� �������� SQL���� ���� ����
	sqlite3_stmt*	mStmt ;
	int				mResultColCount ;

	int				mBindColCount ;
	int				mResultCurrentCol ;

	RESULT_TYPE mResult;

} ;