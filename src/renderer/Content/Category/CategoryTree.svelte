<script lang="ts">
  import { onMount } from 'svelte'
  import { db } from '../../../common/db'
  import { eventer } from '../../../common/eventer'
  import { CategoryModel } from '../../../model/CategoryModel'
  import CategoryTreeNode from './CategoryTreeNode.svelte'
  let categoryTreeMaskVisible = false
  let categorys: CategoryModel[] = []

  let addCategory = (isContextMenuOnNode: boolean) => {
    categoryTreeMaskVisible = true
    let categoryNew = new CategoryModel()
    categoryNew._isNew = true
    categoryNew.hasChild = false
    if (isContextMenuOnNode) {
      eventer.emit('addCategory', categoryNew)
    } else {
      categoryNew.level = 1
      if (categorys.length > 0) {
        let arr = categorys.map((v) => v.order)
        categoryNew.order = Math.max(...arr) + 1
      }
      categorys.splice(0, 0, categoryNew)
      categorys = categorys
    }
  }
  let editCategory = () => {
    categoryTreeMaskVisible = true
    eventer.emit('editCategory')
  }
  let deleteCategory = async () => {
    let index = categorys.findIndex((v) => v.isSelected)
    if (index > -1) {
      if (categorys[index].hasChild) {
        alert('暂时不支持删除包含子类目的分类')
        return
      }
      await db('Category').where({ id: categorys[index].id }).delete()
      categorys.splice(index, 1)
      categorys = categorys
    } else {
      eventer.emit('deleteCategory')
    }
  }
  eventer.on('finishNewCategory', () => {
    categoryTreeMaskVisible = false
  })
  eventer.on('finishEditCategory', () => {
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
        title: '修改分类',
        onClick: editCategory,
      })
      menus.push({
        title: '删除分类',
        onClick: deleteCategory,
      })
    }
    let mousePosition = { x: e.clientX, y: e.clientY }
    eventer.emit('showContextMenu', menus, mousePosition)
  }
  let initCategorys = async () => {
    categorys = await db('Category')
      .where({ parentId: null })
      .orderBy([
        { column: 'order', order: 'desc' },
        { column: 'updateTime', order: 'desc' },
      ])
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
