<script lang="ts"></script>

<div class="categoryTool">
  <div class="titleContent">知识分类</div>
  <div class="addBtn">
    <i class="icon iconaddCategory" />
  </div>
</div>

<style lang="scss">
  .categoryTool {
    height: 38px;
    line-height: 38px;
    padding-left: 8px;
    display: flex;
    background: rgb(250, 250, 250);
    border-right: 1px solid rgb(240, 240, 240);
    border-bottom: 1px solid rgb(240, 240, 240);
  }
  .titleContent {
    padding-left: 8px;
    flex: 1;
  }
  .addBtn {
    width: 26px;
    height: 26px;
    margin: 6px;
    line-height: 26px;
    text-align: center;
    border-radius: 3px;
    color: rgb(40, 167, 69);
    i {
      font-size: 14px;
    }
  }
  .addBtn:hover {
    background-color: rgb(205, 255, 216);
  }
</style>
