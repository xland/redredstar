const electron = require('electron');
const fs = require('fs');
const path = require('path');

import image from './image';
const store = {
    batchUpdate(table, arr) {
        return this.db.transaction(trx => {
            let queries = arr.map(v =>
                this.db(table)
                .where('id', v.id)
                .update(v)
                .transacting(trx)
            );
            return Promise.all(queries)
                .then(trx.commit)
                .catch(trx.rollback);
        });
    },
    imageProcessor: image,
    curArticleId: -1,
}
export default store