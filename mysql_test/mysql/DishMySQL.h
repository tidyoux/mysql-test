#ifndef __dish_MySQL_h__
#define __dish_MySQL_h__

#ifndef WIN32
#include <netinet/in.h>
#else
#include <winsock2.h>
#endif

#include <mysql.h>
#include <string>
#include "../common/Common.h"


class DBMysql
{
public:
    DBMysql( void );
    ~DBMysql( void );
public:
    bool connect();
    void Destroy( void );
    
public:
    int EscapeString( char * to, const char * from );
    int EscapeString( char * to, const void * from );
    
    int32 Query( const char * sql );
    
    int32 getRowCount() const;
    bool GetRow( void );
    
    uint32 getFieldCount() const;
    
    void GetField( uint64 &data, int idx );
    void GetField( int64 &data, int idx );
    
    void GetField( uint32 &data, int idx );
    void GetField( int32 &data, int idx );
    
    void GetField( uint16 &data, int idx );
    void GetField( int16 &data, int idx );
    
    void GetField( uint8 &data, int idx );
    void GetField( int8 &data, int idx );
    
    void GetField( bool &data, int idx );
    void GetField( char &data, int idx );
    
    void GetField( void * buffer, unsigned long &len, int idx );
    void GetField( char * &szString, uint32 &len, int idx );
    
    void GetField( std::string& str, int idx );
    
    void FreeResult( void );
    
    bool realEscapeString(char *to, char *from, unsigned long len);
    
    void Ping();
    
    void AutoCommit(char idx);
    
    void Commit();
    
private:
    bool Create( void );
    
private:
    MYSQL			*m_mysql;
    MYSQL_RES		*m_curResult;
    MYSQL_ROW		m_curRow;
    MYSQL_FIELD		*m_fields;
    
    int32 m_nCurRows;
};




#endif//__dish_MySQL_h__