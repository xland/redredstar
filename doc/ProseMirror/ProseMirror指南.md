> 本文档翻译自 ProseMirror 官方网站，版权属于 ProseMirror 官网，ProseMirror 官网如果对文档做了更新，我不负责更新本翻译文档。
>
> 本文档并不是对官网文档逐字逐句的翻译，翻译过程中加入了我个人的理解，可能会有错误，你如果担心获得了错误的知识，请去官网看英文原文。

# ProseMirror 指南

本指南描述了 ProseMirror 库中使用的各种概念以及它们之间的关系。

为了获得一个完整的印象，建议按照顺序阅读本文档，至少按顺序阅读到`视图组件`小节。

# 简介

ProseMirror 提供了一组用于构建富文本编辑器的工具和概念，用于构建“所见即所得”的文本编辑器，这种编辑器与市面上常见的`样式设置型`编辑器不同。

**ProseMirror 的主要目的是让您的代码可以完全控制文档及文档相关的行为。这里说的文档不是 HTML 文档，而是一个自定义数据结构，它只包含您指定它包含的元素及元素之间的关系。** 所有对文档的更新都经过一个点，在那里你可以检查并处理它们。

核心库不是一个容易使用的组件，**我们将模块化和可定制性的优先级设置的比易用性的优先级高。** 基于此，人们将分发基于 ProseMirror 构建自己的编辑器。因此，ProseMirror 更像是一套乐高积木，而不是火柴盒汽车。

ProseMirror 有 4 个核心模块，做任何编辑工作都需要这 4 个模块，ProseMirror 还有很多扩展模块提供一些挺有用的功能，这些模块也是由官方团队维护的。但这些模块却不是必须的。

核心模块是：

- prosemror-model 定义编辑器的文档数据模型，用于描述用户所编辑的内容的数据结构。
- prosemror-state 用于描述编辑器内数据结构的数据状态，包括选中状态、以及从一个状态移动到下一个状态的事务系统。
- prosemror-view 是一个用户界面组件，该组件将给定的数据状态显示在浏览器中的的一个可编辑元素内，并处理与该元素的用户交互。
- prosemror-transform 模块用于记录和重放用户对文档的修改，这是状态模块（prosemror-state）中事务的基础，并使**撤销操作和协作编辑**成为可能。

此外，在 GitHub ProseMirror 组织下，还有 basic editing commands, binding keys, undo history, input macros, collaborative editing, a simple document schema 等模块。

ProseMirror 不提供浏览器直接可用的脚本，这意味着在使用它时，你可能需要使用某种捆绑器，这样才能把你的脚本与 ProseMirror 的脚本捆扎在一起（顺便也就完成了 tree shaking 的工作）。

## 第一个编辑器

你可以用下面的代码创建一个非常简单的编辑器：

```js
import { schema } from "prosemirror-schema-basic";
import { EditorState } from "prosemirror-state";
import { EditorView } from "prosemirror-view";

let state = EditorState.create({ schema });
let view = new EditorView(document.body, { state });
```

ProseMirror 要求您指定文档遵循的模式，因此它所做的第一件事是导入一个包含基本模式的模块。

接着 ProseMirror 会使用这个基本模式模块创建数据状态，该状态将生成符合该模式的空文档，默认的输入位置（选中区域）被设置到该文档开始处。

最后，为状态创建一个视图，并将其附加到 document.body。

这将把状态文档呈现为可编辑的 DOM 节点，并在用户输入时生成状态事务。

这个编辑器还不是很好用。例如，如果按下 Enter 键，什么也不会发生，因为我们并没有告诉和核心库该如何处理 Enter 键。我们马上就会讲到这个。

## 事务

当用户输入数据或与视图交互时，它会生成“状态事务”。这意味着它不只是就地修改文档并以那种方式隐式更新其状态。相反，每一次更改都会导致创建一个事务，该事务描述对状态所做的更改，并可应用于创建一个新状态，然后使用该状态更新视图。
