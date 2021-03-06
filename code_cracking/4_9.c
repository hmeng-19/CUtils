#include <stdio.h>

#include "bst.h"
#include "list.h"

struct list *merge_seq(struct node *left_seq, struct node *right_seq) {
	struct list *r = NULL;
	struct node *p = NULL;

	if(!left_seq && !right_seq) return NULL;

	//fprintf(stdout, "merge_seq r ");
	r = list_create();

	if(left_seq) {
		struct list *m = merge_seq(left_seq->next, right_seq);
		if(!m) {
			//fprintf(stdout, "merge_seq k ");
			struct list *k = list_create();
			//fprintf(stdout, "merge_seq list_insert_front left_seq->data ");
			list_insert_front(k, (void *)(left_seq->data), TYPE_VOID_POINTER, 0);
			//fprintf(stdout, "merge_seq m ");
			m = list_create();
			//fprintf(stdout, "merge_seq list_insert_front k ");
			list_insert_front(m, (void *) k, TYPE_VOID_POINTER, 0);
		} else {
			p = m->head;
			while(p) {
				//fprintf(stdout, "merge_seq list_insert_front left_seq->data ");
				list_insert_front((struct list *)(p->data), (void *)(left_seq->data), TYPE_VOID_POINTER, 0);
				p = p->next;
			}
		}
		//fprintf(stdout, "merge_seq list_attach ");
		list_attach(r, m);
	}

	if(right_seq) {
		struct list *m = merge_seq(left_seq, right_seq->next);
		if(!m) {
			//fprintf(stdout, "merge_seq k ");
			struct list *k = list_create();
			//fprintf(stdout, "merge_seq list_insert_front right_seq->data ");
			list_insert_front(k, (void *)(right_seq->data), TYPE_VOID_POINTER, 0);
			//fprintf(stdout, "merge_seq m ");
			m = list_create();
			//fprintf(stdout, "merge_seq list_insert_front k ");
			list_insert_front(m, (void *) k, TYPE_VOID_POINTER, 0);
		} else {
			p = m->head;
			while(p) {
				//fprintf(stdout, "merge_seq list_insert_front right_seq->data ");
				list_insert_front((struct list *)(p->data), (void *)(right_seq->data), TYPE_VOID_POINTER, 0);
				p = p->next;
			}
		}
	
		//fprintf(stdout, "merge_seq list_attach ");
		list_attach(r, m);
	}

	return r;
}

struct list *merge_seqs(struct list *left_seqs, struct list *right_seqs) {
	struct list *r = NULL;
	struct node *p = NULL, *q = NULL;
	if(!left_seqs) return right_seqs;
	if(!right_seqs) return left_seqs;
	
	//fprintf(stdout, "merge_seqs -- r ");
	r = list_create();
	p = left_seqs->head;
	while(p) {
		q = right_seqs->head;
		while(q) {
			//fprintf(stdout, "merge_seqs -- list_attach ");
			list_attach(r, merge_seq(((struct list *)(p->data))->head, ((struct list *)(q->data))->head));
			q = q->next;
		}
		p = p->next;
	}

	/* free left_seqs and right_seqs */
	p = left_seqs->head;
	while(p) {
		struct node *next = p->next;
		list_destroy((struct list *)(p->data));
		p = next;
	}
	list_destroy(left_seqs);

	p = right_seqs->head;
	while(p) {
		struct node *next = p->next;
		list_destroy((struct list *)(p->data));
		p = next;
	}
	list_destroy(right_seqs);
	
	return r;
}

struct list *bst_seqs(struct bst *root) {
	struct list *r = NULL;	
	struct list *left_seqs = NULL, *right_seqs = NULL;
	struct node *p = NULL;
	if(!root) return r;

	if(root->left) {
		left_seqs = bst_seqs(root->left);
	}

	if(root->right) {
		right_seqs = bst_seqs(root->right);
	}

	r = merge_seqs(left_seqs, right_seqs);

	if(!r) {
		struct list *item;
		//fprintf(stdout, "bst_seqs -- r ");
		r = list_create();
		//fprintf(stdout, "bst_seqs -- item ");
		item = list_create();
		//fprintf(stdout, "bst_seqs -- list_append root ");
		list_append(item, (void *)root, TYPE_VOID_POINTER, 0);
		//fprintf(stdout, "bst_seqs -- list_append item ");
		list_append(r, (void *)item, TYPE_VOID_POINTER, 0);
		return r;
	}

	p = r->head;
	while(p) {
		//fprintf(stdout, "bst_seqs -- list_insert_front ");
		list_insert_front((struct list *)(p->data), (void *) root, TYPE_VOID_POINTER, 0);
		p = p->next;
	}
	return r;
}

void print_seqs(struct list *r) {
	struct node *p;
	if(!r || !(r->head)) return;

	//fprintf(stdout, "outer list %p\n", (void *)r);

	p = r->head;
	while(p) {
		struct node *n;
		struct list *k = (struct list *)(p->data);
		if(!k || !(k->head)) continue;
		//fprintf(stdout, "inner list %p ", (void *)p);
		n = k->head;
		while(n) {
			//fprintf(stdout, "item%p:", (void *)n);
			struct bst *b = (struct bst *)(n->data);
			if(!b) break;
			//fprintf(stdout, "%d(%p) ", b->data, (void *)b);
			fprintf(stdout, "%d ", b->data);
			n = n->next;
		}
		fprintf(stdout, "\n");
		list_destroy(k);
		p = p->next;
	}
	list_destroy(r);
}

int main(void) {
	//int arr[] = {5, 3, 2, 4, 7, 6};
	//int arr[] = {3, 2, 4};
	int arr[] = {10, 5, 15, 3, 1, 4, 7, 9, 13, 11, 14, 19, 18};
	struct bst *root = bst_build(arr, sizeof(arr)/sizeof(int));
	struct list *r;
	if(!root) return 0;
	//bst_print_inorder(root);
	//fprintf(stdout, "\n");

	r = bst_seqs(root);
	print_seqs(r);

	bst_destroy(root);
	return 0;
}

/* output:
hmeng@hmeng ~/c/cutils/code_cracking$ ./a.out >out
hmeng@hmeng ~/c/cutils/code_cracking$ wc out 
  369600  4804800 12566400 out
*/

// gcc -g3 -Wall -std=c99 -pedantic -iquote ../ ../bst.c 4_9.c ../list.c

/*
==29819== Memcheck, a memory error detector
==29819== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==29819== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==29819== Command: ./a.out
==29819==
==29819==
==29819== HEAP SUMMARY:
==29819==     in use at exit: 0 bytes in 0 blocks
==29819==   total heap usage: 6,916,931 allocs, 6,916,931 frees, 152,072,760 bytes allocated
==29819==
==29819== All heap blocks were freed -- no leaks are possible
==29819==
==29819== For counts of detected and suppressed errors, rerun with: -v
==29819== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
*/

