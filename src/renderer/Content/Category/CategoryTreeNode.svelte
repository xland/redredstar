<script lang="ts">
  import type { ContextMenuEvent } from 'electron'

  import { onMount } from 'svelte'
  import type { CategoryModel } from '../../../model/CategoryModel'
  import { ContextMenuModel } from '../../../model/ContextMenuModel'
  import { categoryStore } from '../../Store/categoryStore'
  import { contextMenuConfigStore, contextMenuStore } from '../../Store/contextMenuStore'
  export let category: CategoryModel
  let eachCategory = (categorys: CategoryModel[]) => {
    for (let element of categorys) {
      if (element.isSelected && element.id != category.id) element.isSelected = false
      if (!element.subCategory || element.subCategory.length < 1) continue
      eachCategory(element.subCategory)
    }
  }
  let categoryClick = () => {
    if (category.isSelected) return
    category.isSelected = true
    categoryStore.update((categorys) => {
      eachCategory(categorys)
      return categorys
    })
  }
  let showContextMenu = (e: MouseEvent) => {
    console.log(e)

    contextMenuStore.update((menus) => {
      menus = []
      let menu1 = new ContextMenuModel()
      menu1.title = '增加子分类'
      let menu2 = new ContextMenuModel()
      menu2.title = '增加同级分类'
      let menu3 = new ContextMenuModel()
      menu3.title = '删除该分类'
      menus.push(menu1)
      menus.push(menu2)
      menus.push(menu3)
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

<div class="categoryItem">
  <div class="line" style={`left:${category.level * 12 + 5}px`} />
  <div on:contextmenu={showContextMenu} on:click|preventDefault={categoryClick} class={`categoryTitle ${category.isSelected ? 'selected' : ''}`} style={`padding-left:${category.level * 12}px`}>
    <div on:click|preventDefault={() => (category.isExpanded = !category.isExpanded)} class="expandBtn">
      <i class={`icon ${category.isExpanded ? 'iconremoveRect' : 'iconaddRect'}`} />
    </div>
    <div class="titleBox">{category.title}</div>
  </div>
  {#if category.subCategory && category.isExpanded}
    {#each category.subCategory as subCategory (subCategory.id)}
      <svelte:self category={subCategory} />
    {/each}
  {/if}
</div>

<style lang="scss">
  .categoryItem {
    line-height: 26px;
    position: relative;
  }
  .line {
    content: ' ';
    position: absolute;
    top: 19px;
    bottom: -8px;
    width: 2px;
    border-left: 1px solid #ddd;
    box-sizing: border-box;
  }

  .categoryItem:last-child > .line {
    bottom: 10px !important;
  }
  .categoryTitle {
    display: flex;
    .expandBtn {
      width: 16px;
      cursor: pointer;
      color: #999;
      //
      i {
        font-size: 10px;
        background: rgb(219, 237, 255);
      }
    }
    .titleBox {
      flex: 1;
    }
  }
  .categoryTitle:hover {
    background: rgba(121, 184, 255, 0.3);
  }
  .selected {
    background: rgb(121, 184, 255) !important;
    color: #fff;
  }
</style>
