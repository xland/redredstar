<script lang="ts">
  import { onMount } from "svelte";
  import ClassicEditor from "@ckeditor/ckeditor5-build-classic";

  onMount(async () => {
    let editorElement = document.getElementById("ckEditor");
    let config = {
      toolbar: {
        items: [
          "heading",
          "|",
          "fontFamily",
          "fontSize",
          "bold",
          "italic",
          "underline",
        ],
      },
    };
    let editor = await ClassicEditor.create(editorElement, config);
    editor.editing.view.change((writer) => {
      console;
      writer.setStyle(
        "height",
        `${window.innerHeight - 80}px`,
        editor.editing.view.document.getRoot()
      );
    });
  });
</script>

<div class="content">
  <div class="category">1</div>
  <div class="titleList">2</div>
  <div class="articleContent">
    <div id="ckEditor" />
  </div>
</div>

<style lang="scss">
  .content {
    display: flex;
    height: calc(100% - 38px);
  }
  .category {
    width: 180px;
    background: rgb(241, 248, 255);
    border-right: 1px solid rgb(220, 228, 235);
  }
  .titleList {
    width: 320px;
    background: rgb(250, 251, 252);
    border-right: 1px solid rgb(230, 231, 232);
  }
  .articleContent {
    flex: 1;
  }
  #ckEditor {
    height: 100%;
  }
</style>
