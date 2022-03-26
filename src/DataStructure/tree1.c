#include "../minishell.h"

t_tree_node	*create_node(t_tree_node *left_child, t_tree_node *right_child, char **command)
{
	t_tree_node *new = malloc(sizeof(t_tree_node));
	if (!new)
		return (NULL);
	if (command)
		new->command = command;
	else
		new->command = NULL;
	new->left = left_child;
	new->right = right_child;
	return (new);
}

t_tree_node*	insert_root(t_tree* tree, char **command, int flag)
{
	t_tree_node *new = create_node(NULL, NULL,  command);
	new->flag = flag;
	tree->root_node = new;
	return (new);
}

t_tree_node*	insert_left(t_tree_node* parent,  char **command, int flag)
{
	t_tree_node *new = create_node(NULL, NULL, command);
	new->flag = flag;
	parent->left = new;
	return (new);
}

t_tree_node*	insert_right(t_tree_node* parent, char **command, int flag)
{
	t_tree_node *new = create_node(NULL, NULL,  command);
	new->flag = flag;
	parent->right= new;
	return (new);
}

// //전위순회
static void		_pre_traverse(t_tree_node *root, char **env)
{
	int backup_std[2];
	int pid;

	// arg[0] = root->left->command;	// ls
	// arg[1] = root->left->argument;
	// if (ft_strncmp(root->left->argument, "", 1) == 0)
	// 	arg[1] = NULL;
	// arg[2] = NULL;

	//탈출조건
	backup_std[0] = dup(0);
	backup_std[1] = dup(1);

	// Redirection
	redir_out(root, root->right);
	//redir_in(root->left_child);

	//check_builtIn : 빌트인명령이라면 부모프로세스에서 수행됩니다.
	if (execute_builtin(root->left->command[0], root->left->command, env) == 0)
	{
		dup2(backup_std[0], 0);
		dup2(backup_std[1], 1);
		return ;
	}

	//자식프로세스 생성
    if ((pid = fork()) == -1) printf("FORK ERROR\n");
    else if (pid != 0)
	{
		// parent's process
        pid = wait(NULL);
    }
    else
	{
		// child's process
		execute(root->left->command[0], root->left->command, env);
		printf("AFTER execute\n");
    }
	dup2(backup_std[0], 0);
	dup2(backup_std[1], 1);
	if (!root->left)
		return ;
	// 재귀
	// _pre_traverse(root->left_child, env);
	// _pre_traverse(root->right_child, env);
	return ;
}


void	pre_traverse(t_tree *tree, char **env)
{
	t_tree_node *root;

	if (!tree)
		return ;
	root = tree->root_node;
	_pre_traverse(root, env);
}

// static void		_in_traverse(t_tree_node *root)
// {
// 	if (!root)
// 		return ;

// 	_in_traverse(root->left);
// 	printf("%s   |", root->command);
// 	printf("%d \n", root->flag);
// 	_in_traverse(root->right);
// 	return ;
// }

// void	in_traverse(t_tree *tree)
// {
// 	t_tree_node *root;

// 	if (!tree)
// 		return ;
// 	root = tree->root_node;
// 	_in_traverse(root);
// }


// static void		_post_traverse(t_tree_node *root)
// {
// 	if (!root)
// 		return ;

// 	_in_traverse(root->left);
// 	_in_traverse(root->right);
// 	printf("%s   |", root->command);
// 	printf("%d \n", root->flag);
// 	return ;
// }

// void	post_traverse(t_tree *tree)
// {
// 	t_tree_node *root;

// 	if (!tree)
// 		return ;
// 	root = tree->root_node;
// 	_post_traverse(root);
// }

// t_tree *createExampleBinTree() {
//     t_tree *pReturn = NULL;
//     t_tree_node *pNodeA = NULL, *pNodeB = NULL, *pNodeC = NULL;
//     t_tree_node *pNodeD = NULL, *pNodeE = NULL, *pNodeF = NULL;
//     t_tree_node *pNodeG = NULL, *pNodeH = NULL, *pNodeI = NULL;
//     t_tree_node *pNodeJ = NULL, *pNodeK = NULL, *pNodeL = NULL;
//     t_tree_node *pNodeM = NULL;
//     t_tree_node node = {0,};


//     pReturn = create_tree();
//     if (pReturn != NULL) {
//         pNodeA = insert_root(pReturn, "A", "ㅂ", 0);

//         pNodeB = insert_left(pNodeA, "B", "ㅈ", 1);

//         pNodeC = insert_right(pNodeA,"C", "ㄷ", 2);
//         if (pNodeB != NULL) {

//             pNodeD = insert_left(pNodeB, "D", "ㄱ", 3);

//             pNodeE = insert_right(pNodeB,"E", "ㅅ", 4);
//         }
//         if (pNodeC != NULL) {

//             pNodeF = insert_left(pNodeC, "F", "ㅛ", 5);

//             pNodeG = insert_right(pNodeC, "G", "ㅕ",6);
//         }
//         if (pNodeD != NULL) {

//             pNodeH = insert_left(pNodeD, "H", "ㅑ", 7);

//             pNodeI = insert_right(pNodeD, "I", "ㅐ", 8);
//         }
//         if (pNodeE != NULL) {

//             pNodeJ = insert_left(pNodeE, "J", "ㅔ", 9);
//         }
//         if (pNodeF != NULL) {

//             pNodeK = insert_right(pNodeF, "K", "ㅁ", 10);
//         }
//         if (pNodeG != NULL) {

//             pNodeL = insert_left(pNodeG, "L", "ㄴ", 11);

//             pNodeM = insert_right(pNodeG, "M", "ㅇ", 12);
//         }
//     }

//     return pReturn;
// }


// int main() {
//     t_tree *pBinTree = NULL;
//     pBinTree = createExampleBinTree();
//     if (pBinTree != NULL) {
//         printf("Preorder Iterative Traversal\n");
// 		printf ("------------------------------------\n");
// 		printf ("cmd | arg | fg\n");
// 		printf ("전위순회\n");
//         pre_traverse(pBinTree);
// 		printf ("중위순회\n");
// 		in_traverse(pBinTree);
// 		printf ("후위순회\n");
// 		post_traverse(pBinTree);
//         delete_tree(pBinTree);
//     }
//     return 0;
// }
