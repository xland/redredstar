<script lang="ts">
  import { eventer } from '../../common/eventer'
  let articleContentSaveStatus = ''
  eventer.on('articleContentNeedSave', () => {
    articleContentSaveStatus = '文档未保存'
  })
  eventer.on('articleContentSaved', () => {
    articleContentSaveStatus = '文档已保存'
  })
</script>

<div class="statusBar">
  <div>共 123 字</div>
  <div>{articleContentSaveStatus}</div>

  <div class="span" />
</div>

<style lang="scss">
  .statusBar {
    height: 28px;
    background: rgb(246, 248, 240);
    border-top: 1px solid #ccc;
    box-sizing: border-box;
    line-height: 28px;
    font-size: 12px;
    display: flex;
    div {
      padding: 0px 12px;
    }
  }
  .span {
    flex: 1;
    padding: 0px !important;
  }
</style>
