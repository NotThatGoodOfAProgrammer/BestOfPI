#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = { 0 };

#define MAX_LINE 128

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb]) return va - vb;	return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
	char line[MAX_LINE];
	while (fgets(line, MAX_LINE, stdin)) {
    ++*nc;
	  ++*nl;
    if ((line[0] != '\n'  &&  line[0] != '\t'  &&  line[0] != ' ')) { ++*nw; }
    for (int i=1; line[i] != '\0'; i++) {
      if ((line[i] != '\n'  &&  line[i] != '\t'  &&  line[i] != ' ') && 
      (line[i-1] == '\n'  ||  line[i-1] == '\t'  ||  line[i-1] == ' ')) {
        ++*nw;
      }
      ++*nc;
    } 
	}
}

void char_count(int char_no, int *n_char, int *cnt) {
  int val = 0;
  
  while (val != EOF) {
    val = getchar();
    if (val >= FIRST_CHAR  &&  val < LAST_CHAR){
      count[val - FIRST_CHAR]++;
    }
  }

  int T[MAX_CHARS];
  for (int i = 0; i < MAX_CHARS; i++) {
    T[i] = i;
  }

  qsort(T, MAX_CHARS, sizeof(int), cmp_di);

  *n_char = T[char_no -1] + FIRST_CHAR;
  *cnt = count[T[char_no -1]];
}

void bigram_count(int bigram_no, int bigram[]) {
  int val = 0;
  int last = 0;
  int flag = 0;
  
  while (val != EOF) {
    val = getchar();
    if (val >= FIRST_CHAR  &&  val < LAST_CHAR){
      if (flag) {
        count[last * MAX_CHARS + val - FIRST_CHAR]++;
      }

      last = val - FIRST_CHAR;

      flag = 1;
    } else { flag = 0; }
  }

  int T[MAX_BIGRAMS];
  for (int i = 0; i < MAX_BIGRAMS; i++) {
    T[i] = i;
  }

  qsort(T, MAX_BIGRAMS, sizeof(int), cmp_di);


  int x = T[bigram_no -1];
  bigram[0] = x /MAX_CHARS + FIRST_CHAR;
  bigram[1] = x %MAX_CHARS + FIRST_CHAR;
  bigram[2] = count[x];
}

void find_comments(int *line_comment_counter, int *block_comment_counter) {
  int last = 0;
  int val = 0;
  int flag = 0;
  
  while (val != EOF) {
    val = getchar();
    if (! flag) {
      if (val == 47  &&  last == 47) {
        ++*line_comment_counter;
        while (val != 10) { val = getchar(); }
      } else if (val == 42  &&  last == 47) {
        ++*block_comment_counter;
        flag = 1;
        val = 0;
      }
    } else if (val == 47  &&  last == 42) { flag = 0; }

    last = val;
  }
}


int read_int() {
	char line[MAX_LINE];
	fgets(line, MAX_LINE, stdin); // to get the whole line
	return (int)strtol(line, NULL, 10);
}

int main(void) {
	int to_do;
	int nl = 0, nw = 0, nc = 0, char_no, n_char, cnt;
	int line_comment_counter = 0, block_comment_counter = 0;
	int bigram[3];

	to_do = read_int();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_int();
			char_count(char_no, &n_char, &cnt);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // bigram_count()
			char_no = read_int();
			bigram_count(char_no, bigram);
			printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

