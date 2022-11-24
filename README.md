# OAuth-Authorization-using-RPC

#### Calcan Elena-Claudia  
#### 343C3  
  
## Synopsys 
------------
- s-a dezvoltat o aplicatie tip client-server care simuleaza un sistem de 
autorizare de tip OAuth, folosind protocolul RPC (Remote Procedure Call)  

## Flow 
------------
- se porneste server-ul si asteapta clientii 
- client-ul face o cerere de autorizare catre server 
- server-ul ii trimite un token pentru autorizarea cererii de acces la resurse
- clientul accepta sau declina token-ul
- clientul face o cerere de acces la resurse
- server-ul ii rapsunde cu un token de acces la resurser
- cu ajutorul acestui token clientul face o cereri catre server asupra unor
operatii pe care sa le faca cu fisiere 
- server-ul ii trimite un raspuns catre client despre cerere (e.g se poate executa,
permission denied, resource not found, token expired etc.) 
- de asemenea, clientul poate face o cerere de reinnoire a token-ului de 
acces la resurse

## Structura proiectului si Implementare 
----------------------------------------

- rpc_auth.x  
    - defineste interfata folosite in comunicarea celor doua entitati 
    - procedurile definite sunt urmatoarele:
        1. cerere de autorizare
        2. cerere pentru aprobarea accesului la resurse 
        3. cerere  pentru jetonul de acces la resurse
        4. cerere pentru efectuarea unei operatii asupra unei resurse
        5. cerere pentru reinnoirea token-ului de acces
        6. cerere pentru aflarea timpul de valabilitate a token-ului  

- server.cpp  
    - reprezinta entry point-ul programului pentru server
    - ia informatiile din baza de date care sunt date ca si argumente a programului
    - in main-ul sau contine instructiunile din urma compilarii stub-ului **svc**  

- server_database.cpp, server_database.h 
    - are rol de baza de date a server-ului
    - contine informatii despre users, tokens, resurse si permisiuni la resurse,
      acestea fiind "relationate intre ele"
    - de asemenea, contine metode de lucru cu aceste informatii (populeaza BD-ul, 
    cauta in BD)  

- authz_server.cpp  
    - repezinta programul server care se ocupa cu partea de autorizare a clientului  
    si de gestionare a jetoanelor  

- resource_server.cpp  
    - reprezinta programul server care se ocupa cu partea de gestionarea a 
    resurselor si a operatiilor pe care le face clientul  

- client.cpp 
    - reprezinta programul care "simuleaza" partea de client
    - este creata doar o singura conexiune la server folosind **clnt_create()**
    - primeste un fisier ca si command line parameter care contine diferite 
    comenzi ale clientilor 
    - programul apeleaza RPC-ul corespunzator comenzii citite si afiseaza 
    raspunsurile primite de la server  

- utils.cpp, utils.h  
    - sunt definite valori si metode folosite la comun de ambele parti  

## Compilare si rulare  
----------------------
- compilarea proiectului se face folosind fisierul **Makefile**,  
cu comanda **make**
- compilarea RPC-ului s-a facut cu ajutorul flag-urilor  
    - *-m*
        - compileaza stub-ul **svc** fara main  
    - *-l*  
        - compileaza stub-ul **clnt**
    - *-c* 
        - compileaza filtrul **xdr**
    - *-h*
        - genereaza **fisierul header**  

- rularea programului server:  
        
        ./server tests/test$n/userIDs.db tests/test$n/resources.db tests/test$n/approvals.db tests/test$n/token_tt
    
    - $n reprezinta numarul testului  

- rularea programului client:  

        ./client tests/test$n/client.in  

    - $n reprezinta numarul testului  
    
- testarea proiectului s-a facut cu ajutorul scriptului *check.sh*, 
pus la dispozitie pe *moodle*  
