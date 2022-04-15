#include "dump.h"
#include "../service/service.h"
#include "../includes/parcinit.h"
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int DryNode (node_t *top, FILE *file)
{
    fprintf (file, "\t\tnode_%p [label = ", top);
    switch (top->ndata.kind)
    {
        case OPERATOR:
            fprintf (file, "\"%c\"];\n", top->ndata.content.token);
            break;
        case FUNCTION:
            fprintf (file, "\"%s\", fillcolor = \"lightgreen\" ];\n", function_names[top->ndata.content.token]);
            break;
        case NUMBER:
            fprintf (file, "\"%lf\", fillcolor = \"mediumpurple1\" ];\n", top->ndata.content.data);
            break;
        case VARIABLE:
            fprintf (file, "\"%s\", fillcolor = \"mediumpurple1\" ];\n", top->ndata.content.var);
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

int NodeGraphInit (node_t *top, FILE *file)
{
    assert (top);
    assert (file);
    if (DryNode (top, file) == ERROR)
        return ERROR;

    if (top->left)
        NodeGraphInit (top->left, file);
    if (top->right)
        NodeGraphInit (top->right, file);

    return NO_ERROR;
}

int GraphDump (node_t *tree)
{
    assert (tree);
    FILE * file = FileOpen ("dump/dump_tree.dot", "w");
    fseek (file, 0L, SEEK_SET);

    fprintf (file,
            "digraph DIFFTORR\n"
            "{\n"
            "\tgraph [dpi = 200, rankdir = \"TB\"];\n\n"
            "\tsubgraph TREE\n"
            "\t{\n"
            "\t\tnode [shape = \"oval\", style = \"filled\", fillcolor = \"lightgoldenrod1\", fontcolor = \"black\", fontsize = \"12\"];\n"
            "\t\tedge [color = \"black\"];\n\n"
    );

    NodeGraphInit (tree, file);
    fprintf (file, "\n");
    NodeGraphLink (tree, file);


    fprintf (file,
            "\t}\n"
            "}\n"
        );

    FileClose (file);
    return NO_ERROR;
}
