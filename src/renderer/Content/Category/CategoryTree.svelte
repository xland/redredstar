<script lang="ts">
  import { onMount } from 'svelte'
  import { dataBase } from '../../../common/dataBase'
  import { eventer } from '../../../common/eventer'
  import { CategoryModel } from '../../../model/CategoryModel'
  import CategoryTreeNode from './CategoryTreeNode.svelte'
  let categoryTreeMaskVisible = false
  let categorys: CategoryModel[] = []

  let addCategory = (isContextMenuOnNode: boolean) => {
    categoryTreeMaskVisible = true
    let categoryNew = new CategoryModel()
    categoryNew._isNew = true
    if (isContextMenuOnNode) {
      eventer.emit('addCategory', categoryNew)
    } else {
      categoryNew.level = 1
      categorys.splice(0, 0, categoryNew)
      categorys = categorys
    }
  }
  let deleteCategory = () => {
    eventer.emit('deleteCategory')
  }
  eventer.on('finishNewCategory', () => {
    categoryTreeMaskVisible = false
  })
  let showContextMenu = (e: MouseEvent) => {
    let target = e.target as HTMLElement
    if (target.classList.contains('categoryTreeMask')) return
    let isContextMenuOnNode = !target.classList.contains('categoryTree')
    let menus = [
      {
        title: '增加分类',
        onClick: () => addCategory(isContextMenuOnNode),
      },
    ]
    if (isContextMenuOnNode) {
      menus.push({
        title: '删除分类',
        onClick: deleteCategory,
      })
    }
    let mousePosition = { x: e.clientX, y: e.clientY }
    eventer.emit('showContextMenu', menus, mousePosition)
  }
  let initCategorys = async () => {
    let db = dataBase.get()
    categorys = await db('Category').orderBy('order', 'desc')
  }
  onMount(() => {
    initCategorys()
  })
</script>

<div class="categoryTree" on:contextmenu={showContextMenu}>
  {#each categorys as category (category.id)}
    <CategoryTreeNode {category} />
  {/each}
  {#if categoryTreeMaskVisible}
    <div class="categoryTreeMask" />
  {/if}
</div>

<style lang="scss">
  .categoryTree {
    background: rgb(219, 237, 255);
    border-right: 1px solid rgb(220, 228, 235);
    flex: 1;
    overflow: auto;
    position: relative;
  }
  .categoryTreeMask {
    position: absolute;
    left: 0px;
    bottom: 0px;
    top: 0px;
    right: 0px;
    z-index: 1;
    background-color: rgba($color: #ffffff, $alpha: 0.6);
  }
</style>
