<script lang="ts">
  import { afterUpdate, onDestroy, onMount, tick } from 'svelte'
  import type { CategoryModel } from '../../../model/CategoryModel'
  import { eventer } from '../../../common/eventer'
  import { db } from '../../../common/db'
  import { globalObjs } from '../../Store/globalObjs'
  export let category: CategoryModel
  let categorys: CategoryModel[] = []
  let inputElement: HTMLElement
  let categoryClick = () => {
    eventer.emit('categorySelected', category.id)
  }
  let categorySelected = async (id) => {
    if (category.id != id && category.isSelected) {
      category.isSelected = false
      await db('Category').update({ isSelected: false }).where({ id: category.id })
    } else if (category.id === id && !category.isSelected) {
      category.isSelected = true
      await db('Category').update({ isSelected: true }).where({ id: category.id })
      eventer.emit('initArticleList', category.id)
    }
  }
  let addCategory = (categoryNew) => {
    if (!category.isSelected) return
    category.isExpanded = true
    categoryNew.parentId = category.id
    categoryNew.levelNum = category.levelNum + 1
    if (categorys.length > 0) {
      let arr = categorys.map((v) => v.orderNum)
      categoryNew.orderNum = Math.max(...arr) + 1
    }
    categorys.splice(0, 0, categoryNew)
    categorys = categorys
  }

  let editCategory = () => {
    if (!category.isSelected) return
    category._isEdit = true
    globalObjs.__tempCategoryTitle = category.title
  }

  let deleteCategory = async () => {
    let index = categorys.findIndex((v) => v.isSelected)
    if (index < 0) return
    if (categorys[index].hasChild) {
      alert('暂时不支持删除包含子类目的分类')
      return
    }
    await db('Category').where({ id: categorys[index].id }).delete()
    categorys.splice(index, 1)
    categorys = categorys
    if (categorys.length < 1) {
      await db('Category').update({ hasChild: false }).where({ id: category.id })
    }
  }
  let finishNewCategory = () => {
    let index = categorys.findIndex((v) => v._isNew)
    if (index > -1) {
      categorys.splice(index, 1)
      categorys = categorys
    }
    if (categorys.length > 0) {
      category.hasChild = true
    }
  }
  let expandBtnVisible = (category: CategoryModel) => {
    if (!category.hasChild) {
      return 'iconremoveRect'
    }
    if (category.isExpanded) {
      return 'iconremoveRect'
    } else {
      return 'iconaddRect'
    }
  }
  let initCategorys = async () => {
    if (category.isExpanded) {
      categorys = await db('Category')
        .where({ parentId: category.id })
        .orderBy([
          { column: 'orderNum', order: 'desc' },
          { column: 'updateTime', order: 'desc' },
        ])
    } else {
      categorys = []
    }
  }
  let expandCategory = async () => {
    if (!category.hasChild) return
    category.isExpanded = !category.isExpanded
    await db('Category').update({ isExpanded: category.isExpanded }).where({ id: category.id })
    initCategorys()
  }
  let categoryTitleInputBlur = async (e: FocusEvent) => {
    let title = category.title.replaceAll(' ', '')
    if (category._isEdit) {
      if (title.length < 1) {
        alert('标题不允许为空')
        category.title = globalObjs.__tempCategoryTitle
      }
      if (title != globalObjs.__tempCategoryTitle) {
        category.title = title
        await db('Category').update({ title, updateTime: Date.now() }).where({ id: category.id })
      }
      category._isEdit = false
      delete globalObjs.__tempCategoryTitle
      eventer.emit('finishEditCategory')
    } else if (category._isNew) {
      if (title.length > 0) {
        category.createTime = Date.now()
        category.updateTime = Date.now()
        if (category.parentId) {
          await db('Category').update({ hasChild: true, isExpanded: true }).where({ id: category.parentId })
        }
        await db('Category').insert(category.getData())
        category._isNew = false
      }
      eventer.emit('finishNewCategory')
    }
  }
  let categoryTitleInputFocus = () => {
    setTimeout(() => {
      if (!inputElement) return
      inputElement.onblur = categoryTitleInputBlur
    }, 180)
  }
  let categoryTitleKeyDown = (e: KeyboardEvent) => {
    if (e.code === 'Enter') {
      inputElement.blur()
    } else if (e.code === 'Escape') {
      category.title = ''
      inputElement.blur()
    }
  }
  onDestroy(() => {
    eventer.off('finishNewCategory', finishNewCategory)
    eventer.off('addCategory', addCategory)
    eventer.off('categorySelected', categorySelected)
    eventer.off('deleteCategory', deleteCategory)
    eventer.off('editCategory', editCategory)
    inputElement = null
  })
  afterUpdate(() => {
    if (inputElement) {
      inputElement.focus()
    }
  })
  onMount(() => {
    eventer.on('finishNewCategory', finishNewCategory)
    eventer.on('addCategory', addCategory)
    eventer.on('categorySelected', categorySelected)
    eventer.on('deleteCategory', deleteCategory)
    eventer.on('editCategory', editCategory)
    initCategorys()
  })
</script>

<div class="categoryItem">
  <div class="line" style={`left:${category.levelNum * 12 + 5}px`} />
  <div on:mousedown={categoryClick} class={`categoryTitle ${category.isSelected ? 'selected' : ''}`} style={`padding-left:${category.levelNum * 12}px`}>
    <div on:mousedown|stopPropagation|preventDefault={expandCategory} class="expandBtn">
      <i class={`icon ${expandBtnVisible(category)}`} />
    </div>
    {#if category._isEdit || category._isNew}
      <div on:mousedown|stopPropagation|preventDefault={() => inputElement.focus()} class="titleInput">
        <input bind:this={inputElement} on:keydown={categoryTitleKeyDown} on:focus={categoryTitleInputFocus} bind:value={category.title} type="text" />
      </div>
    {:else}
      <div class="titleBox">{category.title}</div>
    {/if}
  </div>
  {#each categorys as subCategory (subCategory.id)}
    <svelte:self category={subCategory} />
  {/each}
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
  .titleInput {
    flex: 1;
    position: relative;
    input {
      position: absolute;
      z-index: 2;
      border: 1px solid rgb(33, 136, 255);
      outline: none;
      height: 22px;
      left: 2px;
      right: 6px;
      top: 2px;
      display: block;
      padding: 0px 3px;
    }
  }
</style>
