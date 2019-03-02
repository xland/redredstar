const electron = require('electron');
const fs = require('fs');
const path = require('path');
import db from './db';
import image from './image';
db.init();
const store = {
    basePath: db.basePath,
    db: db.knex,
    imageProcessor:image,
    curArticleId: -1,
}
export default store