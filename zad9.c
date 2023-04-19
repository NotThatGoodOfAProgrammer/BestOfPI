#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;


// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(const pair *relation, int size) {
  int tab[MAX_REL_SIZE] = {0};
  int tab_size = -1;

  for (int i=0; i<size; i++) {
    int frst = relation[i].first, scnd = relation[i].second;

    if (! tab[frst]) {
      tab[frst] = -1;
    }
    if (! tab[scnd]) {
      tab[scnd] = -1;
    }

    if (frst == scnd) {
      tab[relation[i].first] = 1;
    }

    if (frst > tab_size) tab_size = frst;
    if (scnd > tab_size) tab_size = scnd;
  }

  for (int i=0; i<tab_size+1; i++) {
    if (tab[i] == -1) {
      return 0;
    }
  }

  return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(const pair *relation, int size) {
  for (int i=0; i<size; i++) {
    if (relation[i].first == relation[i].second) {
      return 0;
    }
  }

  return 1;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(const pair *relation, int size) {
  for (int i=0; i<size; i++) {
    int flag = 0;
    int frst = relation[i].first, scnd = relation[i].second;

    for (int j=0; j<size; j++) {
      if ((relation[j].first == scnd  &&  relation[j].second == frst)) {
        flag = 1;
        break;
      }
    }

    if (! flag) {
      return 0;
    }
  }

  return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(const pair *relation, int size){
  for (int i=0; i<size; i++) {
    int flag = 1;
    int frst = relation[i].first, scnd = relation[i].second;

    for (int j=i+1; j<size; j++) {
      if ((relation[j].first == scnd  &&  relation[j].second == frst)  &&  frst != scnd) {
        flag = 0;
        break;
      }
    }

    if (! flag) {
      return 0;
    }
  }

  return 1;
};

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(const pair *relation, int size){
  for (int i=0; i<size; i++) {
    int flag = 1;
    int frst = relation[i].first, scnd = relation[i].second;

    for (int j=0; j<size; j++) {
      if (relation[j].first == scnd  &&  relation[j].second == frst) {
        flag = 0;
        break;
      }
    }

    if (! flag) {
      return 0;
    }
  }

  return 1;
};

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(const pair *relation, int size) {
  for (int i=0; i<size; i++) {
    int frst = relation[i].first, scnd = relation[i].second;

    for (int j=0; j<size; j++) {
      if (relation[j].first == scnd) {
        int f_frst = frst, f_scnd = relation[j].second;

        int flag = 0;
        for (int k=0; k<size; k++) {
          if (relation[k].first == f_frst  &&  relation[k].second == f_scnd) {
            flag = 1;
            break;
          }
        }

        if (! flag) {
          return 0;
        }
      }
    }
  }

  return 1;
}

// Case 2:

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(const pair *relation, int size) {
  for (int i=0; i<size; i++) {
    int frst = relation[i].first, scnd = relation[i].second;

    int flag = 1;
    for (int j=i+1; j<size; j++) {
      if ((relation[j].first != scnd  &&  relation[j].second != frst)
      &&  (relation[j].first != frst  ||  relation[j].second != scnd)) {
        flag = 0;
        break;
      }
    }

    if (! flag) {
      return 0;
    }
  }

  return 1;
}

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(const pair *relation, int size) {
  return is_transitive(relation, size) && is_antisymmetric(relation, size) && is_reflexive(relation, size);
}

// A total order relation is a partial order relation that is connected
int is_total_order(const pair *relation, int size) {
  return is_partial_order(relation, size) && is_connected(relation, size);
}


void find_max_elements(const pair *relation, int size, int *max_elements) {
  for (int i=0; i<size; i++) {
    int frst = relation[i].first, scnd = relation[i].second;
    if (max_elements[scnd] != -1) {
      if (frst == scnd) {
        if (max_elements[scnd] == 2) {
          max_elements[scnd] = -1;
        } else {
          max_elements[scnd] = 2;
        }
      } else {
        max_elements[frst] = -1;
      }
      if (max_elements[scnd] == 0) {
        max_elements[scnd] = 1;
      }
    }
  }
}


void find_min_elements(const pair *relation, int size, int *min_elements) {
  for (int i=0; i<size; i++) {
    int frst = relation[i].first, scnd = relation[i].second;
    if (min_elements[frst] != -1) {
      if (frst == scnd) {
        if (min_elements[frst] == 2) {
          min_elements[frst] = -1;
        } else {
          min_elements[frst] = 2;
        }
      } else {
        min_elements[scnd] = -1;
      }
      if (min_elements[frst] == 0) {
        min_elements[frst] = 1;
      }
    }
  }
}


void get_domain(const pair *relation, int size, int *domain) {
  for (int i=0; i<size; i++) {
    int frst = relation[i].first, scnd = relation[i].second;
    
    domain[frst]++;
    domain[scnd]++;
  }
}

// Case 3:

int composition (const pair *relation, int size, const pair *relation_2, int size_2, pair *comp_relation) {
  int cnt = 0;

  for (int i=0; i<size; i++) {
    int scnd = relation[i].second;

    for (int j=0; j<size_2; j++) {
      if (scnd == relation_2[j].first) {
        pair new_pair = {relation[i].first, relation_2[j].second};
        add_relation(comp_relation, &cnt, new_pair);
      }
    }
  }

  return cnt;
}

// Add pair to existing relation if not already there
void add_relation (pair *tab, int *n, pair new_pair) {
  int frst = new_pair.first, scnd = new_pair.second;

  for (int i=0; i<*n; i++) {
    if (tab[i].first == frst  &&  tab[i].second == scnd) {
      return;
    }
  }

  tab[*n] = new_pair;
  ++*n;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
  int n;
  scanf("%d", &n);
  for (int i=0; i<n; i++){
    int a, b;
    scanf("%d %d", &a, &b);

    pair new_struct = {a, b};
    relation[i] = new_struct;
  }

  return n;
}

void print_int_array(int *array) {
  int size = MAX_REL_SIZE;
  int cnt = 0;
  for (int i=0; i<size; i++) {
    if (array[i] > 0) {
      array[cnt++] = i;
    }
  }

  printf("%d\n", cnt);
  for (int i=0; i<cnt; i++) {
    printf("%d ", array[i]);
  }
  printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE] = {0};
	int max_elements[MAX_REL_SIZE] = {0};
	int min_elements[MAX_REL_SIZE] = {0};

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain);
			if (! ordered) break;
			find_max_elements(relation, size, max_elements);
			find_min_elements(relation, size, min_elements);
			print_int_array(max_elements);
			print_int_array(min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

