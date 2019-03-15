export default {
    data() {
        return {
            editorDoc: null,
        }
    },
    methods: {
        initEditorCk(){
            window.CKEDITOR.replace('ckEditor');
        }
    }
}