#MOVIEDB = /afs/andrew.cmu.edu/usr22/seunghak/15826/movies.db
#We will use our full data above for our grading (it is not publicly available)
#Change this test db file to your movie db file
#MOVIEDB = movies-mini.db 
MOVIEDB = movies.db 

#Change this source to your movie data source
SRC = movies-short.csv
#SRC = movies-short.txt
#SRC = [YOUR MOVIE DATA SOURCE]

all: hw1.q1.4 hw1.q1.5 hw1.q1.6 hw1.q1.7

linenum: $(SRC)
	wc -l $(SRC)

hw1.q1.4: 
	sqlite3 $(MOVIEDB) < hw1.q1.4.txt

hw1.q1.5: 
	sqlite3 $(MOVIEDB) < hw1.q1.5.txt

hw1.q1.6: 
	sqlite3 $(MOVIEDB) < hw1.q1.6.txt

hw1.q1.7: 
	sqlite3 $(MOVIEDB) < hw1.q1.7.txt

clean:
	\rm -f *.o
