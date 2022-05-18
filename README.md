# tema3_poo
134_Petcu_Mircea
Tema 3 este o adaptare la tema 2. Am adaugat clasa template<class T> client, unde T este una dintre clasele Card_Standard sau Card_Premium. 
  Design patterns:
  Singleton: clasa Premiu, se da un singur premiu o singura data la  clientul cu card premium cu id -ul 5.Am evidentiat asta in main prin fluxul de testare
  Abstract Factory : clasa Contract ce se ramifica in 2 (Operatii_pasive - imprumuti banca si Operatii_active - banca te imprumuta pe tine)
  Clasa contract este abstracta, iar clasa ContractFactory are o functie statica ce creaza diferite contracte la fiecare client.
  Am pastrat clientii de date in 2 baze de date, 2 vectori de tip client<Card_Standard> si client<Card_Premium>.
  Pentru a selecta fluxul de testare pentru tema 3 se apasa tasta 2. Iar codul de autentificare va fi tot timpul 0.
