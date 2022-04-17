#include "dump.h"
#include "../service/service.h"
#include "../includes/parcinit.h"
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int DryNode (node_t *top, FILE *file, modes mode)
{
    if (mode == DEBUG)
    {
        fprintf (file, "\t\tnode_%p [label = \"top: %p | left: %p | right: %p | ", top, top, top->left, top->right);
        if (top->ndata.kind == VARIABLE)
            fprintf (file, "variable: %p | ", top->ndata.content.var);
    }
    else
        fprintf (file, "\t\tnode_%p [label = \"", top);

    switch (top->ndata.kind)
    {
        case OPERATOR:
            fprintf (file, "%c\"];\n", top->ndata.content.token);
            break;
        case FUNCTION:
            fprintf (file, "%s\", fillcolor = \"lightgreen\" ];\n", function_names[top->ndata.content.token]);
            break;
        case NUMBER:
            fprintf (file, "%lf\", fillcolor = \"mediumpurple1\" ];\n", top->ndata.content.data);
            break;
        case VARIABLE:
            fprintf (file, "%s\", fillcolor = \"mediumpurple1\" ];\n", top->ndata.content.var);
            break;
        default:
            return ERROR;
    }

    return NO_ERROR;
}

int NodeGraphLink (node_t *top, FILE *file)
{
    assert (top);
    assert (file);

    if (top->left)
    {
        fprintf (file, "\t\tnode_%p -> node_%p;\n", top, top->left);
        NodeGraphLink (top->left, file);
    }
    if (top->right)
    {
        fprintf (file, "\t\tnode_%p -> node_%p;\n", top, top->right);
        NodeGraphLink (top->right, file);
    }

    return NO_ERROR;
}

int NodeGraphInit (node_t *top, FILE *file, modes mode)
{
    assert (top);
    assert (file);
    if (DryNode (top, file, mode) == ERROR)
        return ERROR;

    if (top->left)
        NodeGraphInit (top->left, file, mode);
    if (top->right)
        NodeGraphInit (top->right, file, mode);

    return NO_ERROR;
}

int GraphDump (node_t *tree, modes mode)
{
    assert (tree);
    FILE * file = NULL;

    if (mode == DEBUG)
        file = FileOpen ("dump/dump_debug_tree.dot", "w");
    else
        file = FileOpen ("dump/dump_tree.dot", "w");

    fprintf (file,
            "digraph DIFFTORR\n"
            "{\n"
            "\tgraph [dpi = 200, rankdir = \"TB\"];\n\n"
            "\tsubgraph TREE\n"
            "\t{\n"
            "\t\tedge [color = \"black\"];\n\n"
    );

    if (mode == DEBUG)
        fprintf (file, "\t\tnode [shape = record,  style = \"filled\", fillcolor = \"darksalmon\",      fontcolor = \"black\", fontsize = \"12\"];\n");
    else
        fprintf (file, "\t\tnode [shape = \"oval\", style = \"filled\", fillcolor = \"lightgoldenrod1\", fontcolor = \"black\", fontsize = \"12\"];\n");


    NodeGraphInit (tree, file, mode);
    fprintf (file, "\n");
    NodeGraphLink (tree, file);


    fprintf (file,
            "\t}\n"
            "}\n"
        );

    FileClose (file);

    if (mode == DEBUG)
        system ("dot dump/dump_debug_tree.dot -T png -o dump/dump_debug_tree.png");
    else
        system ("dot dump/dump_tree.dot -T png -o dump/dump_tree.png");


    if (mode == OPEN)
        system ("open dump/dump_tree.png");

    return NO_ERROR;
}
