# Example Output

*Note: offsets have been adapted to the WordNet version linked by the task.*

## Task 1
```
./tt1_wordnet query --pos=n --iw=calculator --full
n 09887034 an expert at calculation (or at operating calculating machines)  
	calculator
	reckoner
	figurer
	estimator
	computer

n 02938886 a small machine that is used for mathematical calculations  
	calculator
	calculating_machine
```

### Task 2
```
./tt1_wordnet semrels --pos=n --offset=02938886 --dir=out --full
@ (Hypernym) n 03699975 any mechanical or electrical device that transmits or modifies energy to perform or assist in the performance of human tasks  
	machine

~ (Hyponym) n 02666196 a calculator that performs arithmetic functions by manually sliding counters on rods or in grooves  
	abacus

~ (Hyponym) n 02679142 a machine that adds numbers  
	adder

~ (Hyponym) n 02679257 a calculator that performs simple arithmetic functions  
	adding_machine
	totalizer
	totaliser

~ (Hyponym) n 03116767 a calculator that keeps a record of the number of times something happens  
	counter
	tabulator

~ (Hyponym) n 03483823 a calculator small enough to hold in the hand or carry in a pocket  
	hand_calculator
	pocket_calculator

~ (Hyponym) n 03807334 a set of graduated rods formerly used to do multiplication and division by a method invented by John Napier  
	Napier's_bones
	Napier's_rods

~ (Hyponym) n 04035231 calculator consisting of a cord with attached cords; used by ancient Peruvians for calculating and keeping records  
	quipu

~ (Hyponym) n 04348988 a machine that subtracts numbers  
	subtracter
```

## Task 3
```
./tt1_wordnet shortestpath --pos1=n --offset1=03481172 --pos2=n --offset2=04589745 --full
n 03481172 a hand tool with a heavy rigid head and a handle; used to deliver an impulsive force by striking  
	hammer

@ (Hypernym) n 03489162 a tool used with workers' hands  
	hand_tool

%p (Part meronym) n 03485997 the appendage to an object that is designed to be held in order to use or move it; "he grabbed the hammer by the handle"; "it was an old briefcase but it still had a good grip"  
	handle
	grip
	handgrip
	hold

#p (Part holonym) n 03483086 a rectangular frame with handles at both ends; carried by two people  
	handbarrow

@ (Hypernym) n 03391770 a structure supporting or containing something  
	framework

~ (Hyponym) n 04587648 a framework of wood or metal that contains a glass windowpane and is built into a wall or roof to admit light or air  
	window

%p (Part meronym) n 04589745 a pane of glass in a window; "the ball shattered the window"  
	windowpane
	window
```