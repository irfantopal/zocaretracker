#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <mysql_connection.h> 
#include <mysql_driver.h> 
#include <cppconn/prepared_statement.h>
#include <ctime>

#define UDP_SIZE 8
#define UDP_PORT 4588

#define DB_HOST "tcp://127.0.0.1:3306"
#define DB_USER "DB Username"
#define DB_PASSWORD "DB Password"
#define DB_DATABASE "DB Database"

int main()
{
    int sock, addr_len, bytes_read;
    char recv_data[UDP_SIZE], send_data[UDP_SIZE];
    struct sockaddr_in server_addr, client_addr, client2_addr;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("Bind");
        exit(1);
    }
    
    addr_len = sizeof(struct sockaddr);

    /*
    
    > Example MySQL code which you can use to save the data to a database.
    
    sql::mysql::MySQL_Driver* db_driver;
    sql::Connection* db_conn;
    sql::PreparedStatement* db_stmt;

    db_driver = sql::mysql::get_mysql_driver_instance();
    db_conn = db_driver->connect(DB_HOST, DB_USER, DB_PASSWORD);
    db_conn->setSchema(DB_DATABASE);*/

    printf("\033[2J\033[1;1HZOCareTracker-Server started on port %d\n", UDP_PORT);
    #ifdef _DEBUG
        printf("[DEBUG MODE]\n");
    #endif

    fflush(stdout);

    while (1)
    {
        bytes_read = recvfrom(sock, recv_data, UDP_SIZE, 0,  (struct sockaddr*)&client_addr, (socklen_t*)&addr_len);

        recv_data[bytes_read] = '\0';

        time_t now = time(0);
        tm* ltm = localtime(&now);

        switch (recv_data[0]) {
            case 0:
                printf("%02d.%02d.%d %d:%02d:%02d > BPM: %d | Temp: %dC\n", ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900, ltm->tm_hour, ltm->tm_min, ltm->tm_sec, recv_data[2], recv_data[7]);
                /*
                
                > Example MySQL code which you can use to save the data to a database.
                
                db_stmt = db_conn->prepareStatement("INSERT INTO pulsedata(patientid, bpm, temp) VALUES (?, ?, ?)");
                db_stmt->setInt(1, recv_data[1]);
                db_stmt->setInt(2, recv_data[2]);
                db_stmt->setInt(3, recv_data[7]);
                db_stmt->execute();*/
                break;
            case 2:
                switch (recv_data[2]) {
                    case 2:
                        printf("%02d.%02d.%d %d:%02d:%02d > Patient input: I'm okay\n", ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
                        break;
                    case 3:
                        printf("!!!!!!!!!!!!!!!!!!!!!!!!\n");
                        printf("%02d.%02d.%d %d:%02d:%02d > Patient input: I need help [URGENT]\n", ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
                        printf("!!!!!!!!!!!!!!!!!!!!!!!!\n");
                        break;
                }  
                break;
            case 3:
                switch (recv_data[2]) {
                    case 2:
                        printf("!!!!!!!!!!!!!!!!!!!!!!!!\n");
                        printf("%02d.%02d.%d %d:%02d:%02d > Fall detection [URGENT]\n", ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
                        printf("!!!!!!!!!!!!!!!!!!!!!!!!\n");
                        break;
                }
                break;
        }

        
        #ifdef _DEBUG
            for (int i = 0; i < UDP_SIZE; i++) {
                printf("%x ", (unsigned char)recv_data[i]);
            }
            printf("\n");
        #endif

        fflush(stdout);
    }

    delete db_stmt;
    delete db_conn;

    return 0;
}