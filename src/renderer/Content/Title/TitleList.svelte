<script lang="ts">
  import { afterUpdate, onMount } from 'svelte'
  import { db } from '../../../common/db'
  import { eventer } from '../../../common/eventer'
  import { ArticleModel } from '../../../model/ArticleModel'
  import { globalObjs } from '../../Store/globalObjs'
  let articles: ArticleModel[] = []
  let inputElement: HTMLInputElement
  let titleMaskVisible = false
  let categoryId: string
  let titleClick = (article: ArticleModel) => {
    if (article.isSelected) return
    article.isSelected = true
  }
  let addArticle = () => {
    let article = new ArticleModel()
    article._isNew = true
    articles.splice(0, 0, article)
    articles = articles
    titleMaskVisible = true
    article.categoryId = categoryId
  }
  let editCategory = () => {}
  let deleteCategory = () => {}
  let showContextMenu = (e: MouseEvent) => {
    let target = e.target as HTMLElement
    if (target.classList.contains('categoryTreeMask')) return
    let isContextMenuOnTitle = !target.classList.contains('titleList')
    let menus = [
      {
        title: '增加知识',
        onClick: addArticle,
      },
    ]
    if (isContextMenuOnTitle) {
      menus.push({
        title: '修改标题',
        onClick: editCategory,
      })
      menus.push({
        title: '删除知识',
        onClick: deleteCategory,
      })
    }
    let mousePosition = { x: e.clientX, y: e.clientY }
    eventer.emit('showContextMenu', menus, mousePosition)
  }
  let initArticleList = async (_categoryId?: string) => {
    if (!_categoryId) {
      let category = await db('Category').where({ isSelected: true }).first()
      if (!category) return
      categoryId = category.id
    } else {
      categoryId = _categoryId
    }
    articles = await db('Article').where({ categoryId }).orderBy('updateTime', 'desc')
  }
  let titleKeyDown = (e: KeyboardEvent) => {
    if (e.code === 'Enter') {
      inputElement.blur()
    } else if (e.code === 'Escape') {
      let index = articles.findIndex((v) => v._isNew || v._isEdit)
      articles[index].title = ''
      articles = articles
      inputElement.blur()
    }
  }
  let titleInputBlur = async () => {
    let index = articles.findIndex((v) => v._isNew || v._isEdit)
    let title = articles[index].title.replaceAll(' ', '')
    if (articles[index]._isEdit) {
      if (title.length < 1) {
        alert('标题不允许为空')
        articles[index].title = globalObjs.__tempArticleTitle
      }
      if (title != globalObjs.__tempArticleTitle) {
        articles[index].title = title
        await db('Article').update({ title, updateTime: Date.now() }).where({ id: articles[index].id })
      }
      articles[index]._isEdit = false
      delete globalObjs.__tempCategoryTitle
      articles = articles
    } else if (articles[index]._isNew) {
      if (title.length > 0) {
        articles[index].createTime = Date.now()
        articles[index].updateTime = Date.now()
        articles[index].isSelected = true
        articles[index]._isNew = false
        await db('Article').insert(articles[index].getData())
      } else {
        articles.splice(index, 1)
      }
      articles = articles
    }
  }
  let titleInputFocus = () => {
    setTimeout(() => {
      if (!inputElement) return
      inputElement.onblur = titleInputBlur
    }, 180)
  }
  afterUpdate(() => {
    if (inputElement) {
      inputElement.focus()
    }
  })
  onMount(() => {
    eventer.on('initArticleList', initArticleList)
    initArticleList()
  })
</script>

<div on:contextmenu={showContextMenu} class="titleList">
  {#each articles as article (article.id)}
    {#if article._isEdit || article._isNew}
      <div on:mousedown|stopPropagation|preventDefault={() => inputElement.focus()} class="titleInput">
        <input bind:this={inputElement} on:keydown={titleKeyDown} on:focus={titleInputFocus} bind:value={article.title} type="text" />
      </div>
    {:else}
      <div on:click={() => titleClick(article)} class={`articleTitle ${article.isSelected ? 'selected' : ''}`}>{article.title}</div>
    {/if}
  {/each}

  {#if titleMaskVisible}
    <div class="titleMask" />
  {/if}
</div>

<style lang="scss">
  .titleList {
    background: rgb(241, 248, 255);
    border-right: 1px solid rgb(230, 231, 232);
    flex: 1;
    overflow-y: auto;
    position: relative;
  }
  .articleTitle {
    height: 32px;
    overflow: hidden;
    text-overflow: ellipsis;
    line-height: 32px;
    padding-left: 12px;
  }
  .articleTitle:hover {
    background: rgba(33, 136, 255, 0.2);
  }
  .selected {
    background: rgb(33, 136, 255) !important;
    color: #fff;
  }
  .titleInput {
    flex: 1;
    position: relative;
    height: 32px;
    input {
      position: absolute;
      z-index: 2;
      border: 1px solid rgb(33, 136, 255);
      outline: none;
      height: 22px;
      left: 12px;
      right: 12px;
      top: 5px;
      display: block;
      padding: 0px 3px;
    }
  }
  .titleMask {
    position: absolute;
    left: 0px;
    bottom: 0px;
    top: 0px;
    right: 0px;
    z-index: 1;
    background-color: rgba($color: #ffffff, $alpha: 0.6);
  }
</style>
