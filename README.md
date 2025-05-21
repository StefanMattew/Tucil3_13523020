# Tucil3_13523020

- Penjelasan singkat program yang dibuat.
  Program ini adalah implementasi penyelesaian puzzle Rush Hour menggunakan algoritma pencarian pathfinding. Tujuannya adalah untuk menemukan urutan pergerakan kendaraan seminimal mungkin agar mobil utama ('P') dapat keluar dari papan permainan ('K').
  Dalam permainan ini terdapat 3 opsi dalam algoritma yang digunakan, yakni
  + Greedy Best First Search (GBFS)
    Greedy Best First Search adalah algoritma pathfinding yang memilih jalur yang diperkirakan memiliki cost terendah untuk mencapai target/goal. Algotima ini menggunakan nilai dari fungsi heuristik (h) terkecil sebagai cara untuk pemilihan jalur. Algoritma ini tidak mempertimbangkan cost yang digunakan dari start ke node n. Sehingga fungsi GBFS didefinisikan sebagai f(n) = h(n).
    
  + Uniform Cost Search (UCS)
    UCS adalah algoritma pathfinding yang menggunakan cost dari start sebagai dasar untuk eksplorasi. Algoritma ini menghitung cost akumulatif terkecil dari start node ke node n untuk selanjutnya dapat memilih jalur yang paling efisien. Algoritma UCS dilambangkan sebagai g(n). UCS tidak menggunakan fungsi heuristic dalam pemilihan jalur. Sehingga fungsi UCS didefinisikan sebagai f(n) = g(n).
    
  + A Star (A*)
    Algoritma A star/ A* adalah algoritma patfinding yang menggabungkan GBFS dan UCS untuk mendapatkan jalur dengan cost terkecil. A* membutuhkan fungsi g dan fungsi h sehingga fungsi A* adalah f(n) = g(n) + h(n). A* akan mencari jalur yang mempertimbangkan cost dari start node ke node n dan mempertimbangkan fungsi heuristik dengan aspek jarak yang perlu ditempuh ke goal node.



- Requirement program dan instalasi tertentu bila ada.
  + Sistem Operasi: Windows / Linux
  + Bahasa pemrograman C
  + Compiler gcc
  
- Cara mengkompilasi program bila perlu dikompilasi (pastikan dengan langkah yang jelas dan benar).
  + Pastikan memiliki compiler C yang dibutuhkan untuk program ini (gcc)
  +  Buka terminal pada folder src
  +  Compile dengan perintah
    "gcc -o ../bin/main main.c board.c piece.c priorityqueue.c pathfinder.c hashset.c utils.c -lm"


- Cara menjalankan dan menggunakan program (pastikan dengan langkah yang jelas dan benar).
  + Clone repository ini
  + Buka terminal di folder root ("Tucil3_13523020")
  + Jalankan perintah "./bin/main" pada terminal
  
-  Author / identitas pembuat
    Stefan Mattew Susanto         
    13523020
