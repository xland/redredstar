<script lang="ts">
  import { onMount } from 'svelte'
  import type { ArticleModel } from '../../../model/ArticleModel'
  import { ContextMenuModel } from '../../../model/ContextMenuModel'
  import { articleStore } from '../../Store/articleStore'
  import { contextMenuConfigStore, contextMenuStore } from '../../Store/contextMenuStore'
  let titleClick = (article: ArticleModel) => {
    if (article.isSelected) return
    articleStore.update((articles) => {
      articles.forEach((v) => {
        v.isSelected = v.id === article.id
      })
      return articles
    })
  }
  let showContextMenu = (e: MouseEvent) => {
    contextMenuStore.update((menus) => {
      menus = []
      let menu1 = new ContextMenuModel()
      menu1.title = '增加文章'
      let menu2 = new ContextMenuModel()
      menu2.title = '删除文章'
      menus.push(menu1)
      menus.push(menu2)
      return menus
    })
    contextMenuConfigStore.update((config) => {
      config.visible = true
      config.clientX = e.clientX
      config.clientY = e.clientY
      return config
    })
  }
  onMount(() => {})
</script>

<div class="titleList">
  {#each $articleStore as article (article.id)}
    <div on:contextmenu={showContextMenu} on:click={() => titleClick(article)} class={`articleTitle ${article.isSelected ? 'selected' : ''}`}>{article.title}</div>
  {/each}
</div>

<style lang="scss">
  .titleList {
    background: rgb(241, 248, 255);
    border-right: 1px solid rgb(230, 231, 232);
    flex: 1;
    overflow-y: scroll;
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
</style>
