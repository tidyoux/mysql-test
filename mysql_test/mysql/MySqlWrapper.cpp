#include "MySqlWrapper.h"
#include <iostream>

#define logE(s) \
    std::cout << s << std::endl;

inline int ___atoi(const char* szNumber)
{
    if (szNumber)
        return atoi(szNumber);
    else
        return 0;
}

//inline __int64 ___atoi64(const char* szNumber)
//{
//	if (szNumber)
//		return _atoi64(szNumber);
//	else
//		return 0;
//}

MySqlWrapper::MySqlWrapper( void )
: m_mysql( 0 ),
m_curResult( 0 ),
m_curRow( 0 ),
m_nCurRows( 0 ),
m_fields(NULL)
{
    
}

MySqlWrapper::~MySqlWrapper( void )
{
    Destroy();
}

bool MySqlWrapper::connect()
{
    return Create();
}

bool MySqlWrapper::Create( void )
{
    if ( !m_mysql )
    {
        m_mysql = mysql_init( NULL );
    }
    
    const char *ip = "127.0.0.1";
    const char *userName = "root";
    const char *password = "111111";
    const char *dbName = "default-db";
    if ( m_mysql != mysql_real_connect(m_mysql, ip, userName, password, dbName, 0, NULL, CLIENT_MULTI_STATEMENTS))
    {
        const char *err = mysql_error(m_mysql);
        logE( "Failed to connect the database![" << err <<"]" );
        return false;
    }
    
    if (mysql_set_character_set(m_mysql, "utf8") != 0 )
    {
        logE( "Failed to set character!" );
        return false;
    }
    
    return true;
}


void MySqlWrapper::Destroy( void )
{
    mysql_close(m_mysql);
    m_mysql = NULL;
}

int MySqlWrapper::EscapeString( char * to, const char * from )
{
    if ( m_mysql )
        return (int)mysql_real_escape_string( m_mysql, to, from ,(unsigned long)strlen( from ) );
    else
        return 0;
}

int MySqlWrapper::EscapeString( char * to , const void * from )
{
    if ( m_mysql )
        return (int)mysql_real_escape_string( m_mysql, to, (const char *)from, (unsigned long)strlen( (const char *)from ) );
    else
        return 0;
}

int MySqlWrapper::Query( const char * sql )
{
    if (!m_mysql)
    {
        if (Create())
        {
            logE( "Success to connect the database!" );
        }
        else
        {
            logE( "Failed to connect the database!" );
            return -1;
        }
    }
    
    MYSQL_RES* result;
    if ( 0 != mysql_real_query( m_mysql, sql, (unsigned long)strlen( sql ) ) )
    {
        int nErr = mysql_errno( m_mysql );
        logE( "Failed to query : " << sql );
        logE( "Failed to query database errno : " <<nErr );
        logE( "Error string :" << mysql_error( m_mysql ) );
        
        do
        {
            result = mysql_store_result( m_mysql );
            mysql_free_result(result); 
        } while (!mysql_next_result(m_mysql));
        
        //CR_SERVER_GONE_ERROR CR_SERVER_LOST
        if ( nErr == 2006 || nErr == 2013 )
        {
            logE("Try to reconnect to mysql server");
            if ( Create() )
            {
                logE( "Success to connect the database again!" );
                Ping();
                if ( 0!=mysql_real_query( m_mysql, sql, (unsigned long)strlen( sql ) ) )
                {
                    logE( "Failed to query database again!" );
                    logE( "Errno : "<< mysql_errno( m_mysql ) <<" Error : "<< mysql_error( m_mysql ) );
                    return -1;
                }
            }
            else
            {
                logE( "Failed to connect database!" );
                return -1;
            }
        }
        else if (nErr == 2014)
        {
            logE("Try to clear stored results.");
            MYSQL_RES* result;
            do 
            {
                result = mysql_store_result( m_mysql );
                mysql_free_result(result); 
            } while (!mysql_next_result(m_mysql));
            
            if ( !mysql_real_query( m_mysql, sql, (unsigned long)strlen( sql ) ) )
            {
                logE( "Failed to query database again!" );
                logE( "Errno : "<<mysql_errno( m_mysql )<<"Error : "<< mysql_error( m_mysql ) );
                Create();
                return -1;
            }
        }
        else
        {
            return -1;
        }
    }
    logE("Query Success!");
    
    mysql_free_result( m_curResult );
    m_curResult = mysql_store_result( m_mysql );
    m_nCurRows = (int32)mysql_affected_rows( m_mysql );
    m_curRow = NULL;
    
    if (m_curResult)
    {
        m_fields = mysql_fetch_field(m_curResult);
    }
    else
    {
        m_fields = NULL;
    }
    
    do 
    {
        result = mysql_store_result( m_mysql );
        mysql_free_result(result); 
    } while (!mysql_next_result(m_mysql));
    
    return m_nCurRows;
}

int32 MySqlWrapper::getRowCount() const
{
    return m_nCurRows;
}

bool MySqlWrapper::GetRow( void )
{
    m_curRow = mysql_fetch_row( m_curResult );
    return (m_curRow != NULL) && (*m_curRow != NULL);
}

uint32 MySqlWrapper::getFieldCount() const
{
    return mysql_num_fields(m_curResult);
}

//void MySqlWrapper::GetField( UINT64 &data, int idx )
//{
//    if ( m_curRow )
//        data = (UINT64)( ___atoi64( m_curRow[idx] ) & 0xffffffffffffffff );
//    else
//        data = 0;
//}

//void MySqlWrapper::GetField( INT64 &data, int idx )
//{
//    if ( m_curRow )
//        data = (INT64)( ___atoi64( m_curRow[idx] ) & 0xffffffffffffffff );
//    else
//        data = 0;
//}

void MySqlWrapper::GetField( uint32 &data, int idx )
{
    if ( m_curRow )
        data = (uint32)( ___atoi( m_curRow[idx] ) & 0xffffffff );
    else
        data = 0;
}

void MySqlWrapper::GetField( int32 &data, int idx )
{
    if ( m_curRow )
        data = (int32)( ___atoi( m_curRow[idx] ) & 0xffffffff );
    else
        data = 0;
}

void MySqlWrapper::GetField( uint16 &data, int idx )
{
    if ( m_curRow )
        data = (uint16)( ___atoi( m_curRow[idx] ) & 0xffff );
    else
        data = 0;
}

void MySqlWrapper::GetField( int16 &data, int idx )
{
    if ( m_curRow )
        data = (int16)( ___atoi( m_curRow[idx] ) & 0xffff );
    else
        data = 0;
}

void MySqlWrapper::GetField( uint8 &data, int idx )
{
    if ( m_curRow )
        data = (uint8)( ___atoi( m_curRow[idx] ) & 0xff );
    else
        data = 0;
}

void MySqlWrapper::GetField( int8 &data, int idx )
{
    if ( m_curRow )
        data = (int8)( ___atoi( m_curRow[idx] ) & 0xff );
    else
        data = 0;
}

void MySqlWrapper::GetField( bool &data, int idx )
{
    if ( m_curRow )
        data = 0!=(___atoi( m_curRow[idx] ) & 0xff );
    else
        data = false;
}

void MySqlWrapper::GetField( char &data, int idx )
{
    if ( m_curRow )
        data = (char)( ___atoi( m_curRow[idx] ) & 0xff );
    else
        data = 0;
}

void MySqlWrapper::GetField( void * buffer, unsigned long &len, int idx )
{
    len = 0;
    if ( m_curRow && m_fields )
    {
        len = *mysql_fetch_lengths(m_curResult);
        memcpy( buffer, m_curRow[idx], len );
    }
}

void MySqlWrapper::GetField( char * &szString, uint32 &len, int idx )
{
    if (m_curRow[idx] != NULL)
    {
        unsigned long *FieldLength = mysql_fetch_lengths(m_curResult);
        len = (uint32)FieldLength[idx];
        szString = (char*)(m_curRow[idx]);
    }
}

void MySqlWrapper::GetField( std::string& str, int idx )
{
    if ( m_curRow )
    {
        if (m_curRow[idx] !=NULL)
            str = m_curRow[idx];
    }
}

void MySqlWrapper::FreeResult( void )
{
    mysql_free_result( m_curResult );
}

bool MySqlWrapper::realEscapeString( char *to, char *from, unsigned long len )
{
    if (!m_mysql)
        return false;
    
    mysql_real_escape_string(m_mysql, to, from, len);
    return true;
}

void MySqlWrapper::Ping()
{
    mysql_ping(m_mysql);
}

void MySqlWrapper::AutoCommit(char idx)
{
    mysql_autocommit(m_mysql, idx);
}

void MySqlWrapper::Commit()
{
    mysql_commit(m_mysql);
}





