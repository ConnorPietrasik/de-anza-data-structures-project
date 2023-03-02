# CIS22C Project
In this group project, we simulated a very simple store. We generally weren't allowed to include much (aside from the basics like iostream and string), but I asked if I could include Windows.h and sstream. I then added a GUI, entirely in C++ using only those additional headers, because I thought it would be fun and wanted to challenge myself. Please see the presentation pdf for charts and diagrams.
  
## Usage
When you launch the program, it looks for "Fry_Database.tsv" in the same folder with the exe. If it isn't found, a file selection window will pop up where you can select a different tsv file to be used for storing the product info. You can also choose to cancel, which will cause it to begin empty and you'll have to add any products manually.
![image](https://user-images.githubusercontent.com/56143403/222348975-d5392a1c-aade-4cac-a8d5-951b6a7f743e.png)


Once it's loaded, you can add new products, delete products, search for products, list products in alphabetical order, and view your current cart (you can also list products in hash-table order, show a visual representation of the internal BST, and view the efficiency of the hash table). Buying products reduces the number in stock, and if it falls to 0 it takes stock from storage. Storage is intentionally allowed to be negative.
![image](https://user-images.githubusercontent.com/56143403/222349567-54b3bda6-80c8-4d95-a94b-26fa7ab8ff97.png)
![image](https://user-images.githubusercontent.com/56143403/222349794-8ac876b7-8393-47b6-a10e-7d1b2c4acef8.png)
![image](https://user-images.githubusercontent.com/56143403/222349870-f105aa97-8edd-4908-940f-3d7706624811.png)
![image](https://user-images.githubusercontent.com/56143403/222350260-5c822c14-79ef-4812-8076-1453378eec05.png)


Once you exit the program, it will save to the tsv file selected at the beginning (or the default, if applicable). If you didn't choose a tsv file, it will ask you where you want it to be saved.
