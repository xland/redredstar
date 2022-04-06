<script lang="ts">
  import { onMount } from 'svelte'
  import { CategoryModel } from '../../../model/CategoryModel'
  import { eventer } from '../../../common/eventer'
  export let category: CategoryModel
  let categorys: CategoryModel[] = []
  let inputElement: HTMLElement
  let categoryClick = () => {
    eventer.emit('categorySelected', category.id)
  }
  eventer.on('categorySelected', (id) => {
    if (category.id != id) category.isSelected = false
    else category.isSelected = true
  })
  eventer.on('addCategory', (categoryNew) => {
    if (!category.isSelected) return
    category.isExpanded = true
    categoryNew.level = category.level + 1
    categorys.splice(0, 0, categoryNew)
    categorys = categorys
  })
  eventer.on('finishNewCategory', () => {
    let index = categorys.findIndex((v) => v.isNew)
    if (index > -1) {
      categorys.splice(index, 1)
      categorys = categorys
    }
  })
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
  let initCategorys = () => {
    if (!category.isExpanded) return
    for (let x = 0; x < 6; x++) {
      let item = new CategoryModel()
      item.id = `${category.id}_${x}`
      item.title = `分类${item.id}`
      item.order = x
      item.level = category.level + 1
      categorys.push(item)
    }
    categorys = categorys
  }
  let expandCategory = () => {
    category.isExpanded = !category.isExpanded
    if (category.isExpanded) {
      initCategorys()
    } else {
      categorys = []
    }
  }
  let categoryTitleInputBlur = (e: FocusEvent) => {
    console.log(1)
    let title = category.title.replaceAll(' ', '')
    if (title.length > 0) {
      category.isNew = false
    }
    eventer.emit('finishNewCategory')
    inputElement.removeEventListener('blur', categoryTitleInputBlur)
  }
  let categoryTitleInputFocus = () => {
    inputElement.addEventListener('blur', categoryTitleInputBlur)
  }
  onMount(() => {
    initCategorys()
    if (inputElement) {
      setTimeout(() => {
        inputElement.focus()
      }, 500)
    }
  })
</script>

<div class="categoryItem">
  <div class="line" style={`left:${category.level * 12 + 5}px`} />
  <div on:mousedown={categoryClick} class={`categoryTitle ${category.isSelected ? 'selected' : ''}`} style={`padding-left:${category.level * 12}px`}>
    <div on:mousedown|stopPropagation|preventDefault={expandCategory} class="expandBtn">
      <i class={`icon ${expandBtnVisible(category)}`} />
    </div>
    {#if category.isNew}
      <div on:mousedown|stopPropagation|preventDefault={() => false} class="titleInput">
        <input bind:this={inputElement} on:focus={categoryTitleInputFocus} bind:value={category.title} type="text" />
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
