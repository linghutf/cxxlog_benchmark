# Git操作

## 基本操作

1. 创建仓库`git init`
2. 检出仓库`git clone URL`
3. 工作流,本地仓库由git维护的三棵"树"组成
  - 工作目录，包含实际文件
  - 暂存区,临时保存改动
  - `HEAD`,指向最后一次提交
4. 添加文件`git add file`,添加所有最后一次更改`git add -A`,添加到暂存区
5. 提交更改`git commit -m "update message."`,提交到了HEAD
6. 推送到远程仓库`git push 'origin branch'`
7. 添加本地仓库到远程`git remote add origin <server>`
8. 更新本地仓库`git pull`
9. 合并远程的改动`git fetch`,`git merge`
10. 查看版本差异`git diff <source_branch> <target_branch>`
11. 将差异暂存`git stash`
12. 查看版本日志`git log`

## 进阶操作

- 创建分支并切换`git checkout -b new_branch`
- 删除分支`git brache -d new_branch`
- 推送分支 `git push origin new_branch`

- 添加标签`git tag <tag_name> <log_id>`,为版本添加标签

## 撤销操作

- 取消对文件的修改 `git checkout -- <filename>`
- 修改最后一次提交`git commit -amend`
- 放弃上一次commit `git reset --hard HEAD^`