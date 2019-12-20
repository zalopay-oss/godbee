make: 	KVstruct.h utils.h utils.cpp DataFileIO.h DataFileIO.cpp FileManager.h FileManager.cpp Comparator.h Comparator.cpp BTreeNode.h BTreeNode.cpp BTree.h BTree.cpp KeyValueStore.h KeyValueStore.cpp main.cpp
		#g++ -std=c++14 -o btree main.cpp KeyValueStore.cpp BTree.cpp BTreeNode.cpp Comparator.cpp DataFileIO.cpp utils.cpp
		#g++ -Wall -g3 -fsanitize=address -std=c++14 -o btree main.cpp KeyValueStore.cpp BTree.cpp BTreeNode.cpp Comparator.cpp DataFileIO.cpp utils.cpp
		g++ -g -rdynamic -std=c++17 -o btree main.cpp KeyValueStore.cpp BTree.cpp BTreeNode.cpp Comparator.cpp FileManager.cpp DataFileIO.cpp utils.cpp	-pthread
		#g++ -std=c++17 -g -o test KeyValueStore.cpp BTree.cpp BTreeNode.cpp utils.cpp DataFileIO.h DataFileIO.cpp FileManager.h FileManager.cpp Comparator.h Comparator.cpp test.cpp -pthread

clean:
		rm btree

#catch.hpp test.cpp