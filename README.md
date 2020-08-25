# CIS22C Project
In this group project, we simulated a very simple store. We generally weren't allowed to include much (aside from the basics like <iostream> and <string>), but I asked if I could include <Windows.h> and <sstream>. I then added a GUI, entirely in C++ using only those additional headers, because I thought it would be fun and wanted to challenge myself. For a detailed breakdown of who did what, as well as visualizations of the data structures, please see the presentation pdf.
  
## Usage
When you launch the program, it looks for "Fry_Database.tsv" in the same folder with the exe. If it isn't found, a file selection window will pop up where you can select a different tsv file to be used for storing the product info. You can also choose to cancel, which will cause it to begin empty and you'll have to add any products manually.

Once it's loaded, you can add new products, delete products, search for products, list products in alphabetical order, and view your current cart (you can also list products in hash-table order, show a visual representation of the internal BST, and view the efficiency of the hash table). These are all pretty self-explanatory, but note that you may have to close and reopen a window to refresh it if you changed the product list with a different window (he didn't allow using <thread>). Buying products reduces the number in stock, and if it falls to 0 it takes stock from storage. Storage is allowed to be negative.

Once you exit the program, it will save to the tsv file selected at the beginning (or the default, if applicable). If you didn't choose a tsv file, it will ask you where you want it to be saved.
